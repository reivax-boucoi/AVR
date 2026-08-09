#!/usr/bin/env python3
"""
ECG2.py

PPG / pulse-sensor BPM and HRV analysis pipeline.

Input CSV:
    HostTime_ns,Data
    1754774325123456789,640
    1754774325128456790,638
    ...

Outputs:
    01_signal_quality.html
    02_heart_rate.html
    03_hrv.html

Main processing stages:
    1. Timestamp analysis
    2. Uniform resampling
    3. Band-pass filtering
    4. Signal-quality analysis
    5. Candidate pulse detection
    6. Accepted/rejected peak classification
    7. BPM calculation
    8. HR distribution
    9. HRV analysis

Dependencies:
    numpy
    pandas
    scipy
    plotly

Install:
    pip install numpy pandas scipy plotly
"""

import argparse
from pathlib import Path

import numpy as np
import pandas as pd

from scipy import signal
from scipy import ndimage

import plotly.graph_objects as go
from plotly.subplots import make_subplots


# ============================================================
# CONFIGURATION
# ============================================================

DEFAULT_FS = 190.0

# Pulse/PPG band.
#
# 0.5 Hz = 30 BPM
# 5.0 Hz = 300 BPM
#
# The accepted physiological HR range is much narrower.

FILTER_LOW_HZ = 0.50
FILTER_HIGH_HZ = 5.00
FILTER_ORDER = 4


# Signal-quality windows.

QUALITY_WINDOW_SEC = 8.0
QUALITY_STEP_SEC = 2.0


# Heart-rate trend.

HR_TREND_SEC = 30.0


# Physiological pulse range.

MIN_BPM = 40.0
MAX_BPM = 180.0

MIN_RR_SEC = 60.0 / MAX_BPM
MAX_RR_SEC = 60.0 / MIN_BPM


# Candidate peak detection.

MIN_PEAK_DISTANCE_SEC = 0.35

# Peak prominence relative to robust signal scale.

PEAK_PROMINENCE_FACTOR = 0.20


# ============================================================
# SQI THRESHOLDS
# ============================================================

# IMPORTANT:
#
# These thresholds are used BOTH for:
#   - classification
#   - horizontal lines on the SQI plot
#
# Therefore there cannot be a discrepancy between what the
# plot says and what the console classification says.

SQI_GOOD = 0.60
SQI_QUESTIONABLE = 0.35


# SNR is informational.
#
# It is NOT used to downgrade SQI.

SNR_GOOD_DB = 3.0
SNR_QUESTIONABLE_DB = 0.0


# RR artifact rejection.

RR_LOCAL_DEVIATION = 0.25


# Minimum number of intervals needed for HRV.

MIN_HRV_INTERVALS = 5


# ============================================================
# CSV
# ============================================================

def load_csv(path: Path) -> pd.DataFrame:
    """
    Load logger CSV.
    """

    df = pd.read_csv(
        path,
        usecols=["HostTime_ns", "Data"]
    )

    df["HostTime_ns"] = pd.to_numeric(
        df["HostTime_ns"],
        errors="coerce"
    )

    df["Data"] = pd.to_numeric(
        df["Data"],
        errors="coerce"
    )

    df = df.dropna(
        subset=[
            "HostTime_ns",
            "Data"
        ]
    )

    df = df.sort_values(
        "HostTime_ns"
    )

    # Remove duplicate timestamps.
    df = df.drop_duplicates(
        subset="HostTime_ns",
        keep="first"
    )

    df = df.reset_index(
        drop=True
    )

    return df


# ============================================================
# TIMESTAMP ANALYSIS
# ============================================================

def timestamp_analysis(
    df: pd.DataFrame,
    nominal_fs: float
):
    ts_ns = df[
        "HostTime_ns"
    ].to_numpy(
        dtype=np.int64
    )

    if len(ts_ns) < 2:
        raise RuntimeError(
            "Not enough samples."
        )

    dt_ns = np.diff(ts_ns)
    dt = dt_ns / 1e9

    duration = (
        ts_ns[-1] -
        ts_ns[0]
    ) / 1e9

    n = len(ts_ns)

    estimated_fs = (
        (n - 1) / duration
        if duration > 0
        else np.nan
    )

    median_dt = np.median(dt)
    mean_dt = np.mean(dt)
    std_dt = np.std(dt)

    expected_dt = 1.0 / nominal_fs

    large_gap_mask = (
        dt > 1.5 * expected_dt
    )

    large_gaps = dt[
        large_gap_mask
    ]

    missing_est = int(
        np.round(
            (
                large_gaps /
                expected_dt
            ) - 1
        ).clip(
            min=0
        ).sum()
    )

    print()
    print("=" * 60)
    print("TIMESTAMP ANALYSIS")
    print("=" * 60)

    print(
        f"Samples                 : {n}"
    )

    print(
        f"Duration                : "
        f"{duration:.3f} s"
    )

    print(
        f"Duration                : "
        f"{duration / 60:.2f} min"
    )

    print(
        f"Nominal Fs              : "
        f"{nominal_fs:.3f} Hz"
    )

    print(
        f"Estimated Fs            : "
        f"{estimated_fs:.3f} Hz"
    )

    print(
        f"Mean dt                 : "
        f"{mean_dt * 1000:.4f} ms"
    )

    print(
        f"Median dt               : "
        f"{median_dt * 1000:.4f} ms"
    )

    print(
        f"Std dt                  : "
        f"{std_dt * 1000:.4f} ms"
    )

    print(
        f"Min dt                  : "
        f"{dt.min() * 1000:.4f} ms"
    )

    print(
        f"Max dt                  : "
        f"{dt.max() * 1000:.4f} ms"
    )

    print(
        f"Large gaps              : "
        f"{len(large_gaps)}"
    )

    print(
        f"Estimated missing samples : "
        f"{missing_est}"
    )

    return {
        "timestamps_ns": ts_ns,
        "dt": dt,
        "duration": duration,
        "estimated_fs": estimated_fs,
        "large_gap_mask": large_gap_mask,
        "large_gaps": large_gaps,
        "missing_est": missing_est,
    }


# ============================================================
# UNIFORM RESAMPLING
# ============================================================

def resample_to_uniform(
    timestamps_ns,
    values,
    fs
):
    """
    Resample timestamped data onto a uniform grid.

    Short irregularities are interpolated.

    Large acquisition gaps are NOT interpolated across.
    Those regions are left as NaN.
    """

    t = (
        timestamps_ns -
        timestamps_ns[0]
    ) / 1e9

    duration = t[-1]

    n_uniform = int(
        np.floor(
            duration * fs
        )
    ) + 1

    tu = (
        np.arange(
            n_uniform,
            dtype=float
        ) / fs
    )

    yu = np.full(
        len(tu),
        np.nan,
        dtype=float
    )

    expected_dt = 1.0 / fs

    gap_idx = np.where(
        np.diff(t) >
        1.5 * expected_dt
    )[0]

    starts = np.r_[
        0,
        gap_idx + 1
    ]

    ends = np.r_[
        gap_idx,
        len(t) - 1
    ]

    for start, end in zip(
        starts,
        ends
    ):

        if end <= start:
            continue

        ts = t[
            start:end + 1
        ]

        vs = values[
            start:end + 1
        ]

        mask = np.isfinite(vs)

        if np.sum(mask) < 2:
            continue

        ts = ts[mask]
        vs = vs[mask]

        u_mask = (
            (tu >= ts[0]) &
            (tu <= ts[-1])
        )

        if not np.any(u_mask):
            continue

        yu[
            u_mask
        ] = np.interp(
            tu[u_mask],
            ts,
            vs
        )

    return tu, yu


# ============================================================
# FILTER
# ============================================================

def butter_bandpass(
    x,
    fs,
    lowcut,
    highcut,
    order=4
):
    """
    Zero-phase Butterworth band-pass.

    NaN regions are preserved.

    Filtering is performed independently on contiguous
    finite regions.
    """

    x = np.asarray(
        x,
        dtype=float
    )

    y = np.full_like(
        x,
        np.nan
    )

    nyq = fs / 2.0

    low = lowcut / nyq
    high = highcut / nyq

    if not (
        0 < low < high < 1
    ):
        raise ValueError(
            "Invalid filter frequencies."
        )

    sos = signal.butter(
        order,
        [
            low,
            high
        ],
        btype="band",
        output="sos"
    )

    finite = np.isfinite(x)

    labels, count = ndimage.label(
        finite
    )

    for label_id in range(
        1,
        count + 1
    ):

        idx = np.where(
            labels ==
            label_id
        )[0]

        # sosfiltfilt requires a reasonably long segment.
        if len(idx) < (
            3 * (
                2 * order + 1
            )
        ):
            continue

        segment = x[
            idx
        ]

        try:

            filtered = signal.sosfiltfilt(
                sos,
                segment
            )

            y[
                idx
            ] = filtered

        except ValueError:
            continue

    return y


# ============================================================
# ROBUST SIGNAL SCALE
# ============================================================

def robust_std(x):

    x = np.asarray(
        x,
        dtype=float
    )

    x = x[
        np.isfinite(x)
    ]

    if len(x) == 0:
        return np.nan

    med = np.median(x)

    mad = np.median(
        np.abs(
            x - med
        )
    )

    return 1.4826 * mad


# ============================================================
# QUALITY METRICS
# ============================================================

def spectral_quality(
    x,
    fs
):
    """
    Calculate:
        - SNR
        - cardiac-band power fraction
        - periodicity

    SNR is intentionally independent from SQI.
    """

    x = np.asarray(
        x,
        dtype=float
    )

    x = x[
        np.isfinite(x)
    ]

    if len(x) < int(
        fs * 4
    ):
        return (
            np.nan,
            np.nan,
            np.nan
        )

    x = signal.detrend(
        x
    )

    freqs, psd = signal.periodogram(
        x,
        fs=fs,
        window="hann",
        detrend="constant",
        scaling="density"
    )

    # Physiological cardiac range.
    cardiac_mask = (
        (freqs >= MIN_BPM / 60.0) &
        (freqs <= MAX_BPM / 60.0)
    )

    if not np.any(
        cardiac_mask
    ):
        return (
            np.nan,
            np.nan,
            np.nan
        )

    # Narrower band representing the useful pulse region.
    signal_mask = (
        (freqs >= 0.7) &
        (freqs <= 3.0)
    )

    total_mask = (
        (freqs >= 0.3) &
        (freqs <= 8.0)
    )

    cardiac_power = np.trapezoid(
        psd[
            signal_mask
        ],
        freqs[
            signal_mask
        ]
    )

    total_power = np.trapezoid(
        psd[
            total_mask
        ],
        freqs[
            total_mask
        ]
    )

    if total_power <= 0:
        cardiac_fraction = 0.0
    else:
        cardiac_fraction = (
            cardiac_power /
            total_power
        )

    noise_mask = (
        total_mask &
        ~signal_mask
    )

    noise_power = np.trapezoid(
        psd[
            noise_mask
        ],
        freqs[
            noise_mask
        ]
    )

    if (
        cardiac_power > 0 and
        noise_power > 0
    ):
        snr_db = (
            10.0 *
            np.log10(
                cardiac_power /
                noise_power
            )
        )
    else:
        snr_db = np.nan

    # --------------------------------------------------------
    # Periodicity
    # --------------------------------------------------------

    centered = (
        x -
        np.mean(x)
    )

    ac = signal.correlate(
        centered,
        centered,
        mode="full",
        method="fft"
    )

    ac = ac[
        len(ac) // 2:
    ]

    if (
        len(ac) == 0 or
        ac[0] <= 0
    ):
        periodicity = 0.0

    else:

        ac /= (
            ac[0] + 1e-12
        )

        lag_min = int(
            fs * MIN_RR_SEC
        )

        lag_max = min(
            len(ac) - 1,
            int(
                fs * MAX_RR_SEC
            )
        )

        if lag_max > lag_min:

            periodicity = float(
                np.max(
                    ac[
                        lag_min:
                        lag_max + 1
                    ]
                )
            )

        else:

            periodicity = 0.0

    return (
        float(snr_db),
        float(cardiac_fraction),
        float(periodicity)
    )


# ============================================================
# SQI
# ============================================================

def calculate_sqi(
    cardiac_power,
    periodicity,
    ac_rms
):
    """
    SQI is deliberately constructed from:
        - cardiac-band dominance
        - periodicity
        - presence of a non-flat waveform

    SNR is NOT included.

    SQI range:
        0 ... 1
    """

    if not np.isfinite(
        cardiac_power
    ):
        return np.nan

    if not np.isfinite(
        periodicity
    ):
        return np.nan

    # Cardiac power score.
    power_score = np.clip(
        cardiac_power / 0.75,
        0.0,
        1.0
    )

    # Periodicity score.
    periodicity_score = np.clip(
        periodicity,
        0.0,
        1.0
    )

    # Flat-signal sanity check.
    if (
        not np.isfinite(ac_rms) or
        ac_rms <= 0
    ):
        level_score = 0.0
    else:
        level_score = 1.0

    sqi = (
        0.45 * power_score +
        0.45 * periodicity_score +
        0.10 * level_score
    )

    return float(
        np.clip(
            sqi,
            0.0,
            1.0
        )
    )


def classify_sqi(sqi):
    """
    SQI classification:

        SQI >= 0.60 : GOOD
        SQI >= 0.35 : QUESTIONABLE
        SQI <  0.35 : BAD

    These exact thresholds are also used by the plot.
    """

    if not np.isfinite(
        sqi
    ):
        return "BAD"

    if sqi >= SQI_GOOD:
        return "GOOD"

    if sqi >= SQI_QUESTIONABLE:
        return "QUESTIONABLE"

    return "BAD"


# ============================================================
# QUALITY WINDOWS
# ============================================================

def analyze_quality(
    t,
    raw,
    filtered,
    fs
):

    window_n = int(
        QUALITY_WINDOW_SEC *
        fs
    )

    step_n = int(
        QUALITY_STEP_SEC *
        fs
    )

    records = []

    if window_n < 10:
        raise RuntimeError(
            "Quality window too short."
        )

    for start in range(
        0,
        len(filtered) - window_n + 1,
        step_n
    ):

        end = (
            start +
            window_n
        )

        raw_w = raw[
            start:end
        ]

        filt_w = filtered[
            start:end
        ]

        finite = (
            np.isfinite(raw_w) &
            np.isfinite(filt_w)
        )

        if np.sum(finite) < (
            0.8 * window_n
        ):
            continue

        raw_valid = raw_w[
            finite
        ]

        filt_valid = filt_w[
            finite
        ]

        # AC RMS of filtered cardiac signal.
        ac_rms = float(
            np.sqrt(
                np.mean(
                    filt_valid ** 2
                )
            )
        )

        (
            snr_db,
            cardiac_power,
            periodicity
        ) = spectral_quality(
            filt_valid,
            fs
        )

        sqi = calculate_sqi(
            cardiac_power,
            periodicity,
            ac_rms
        )

        classification = classify_sqi(
            sqi
        )

        records.append({
            "time": (
                t[start] +
                t[end - 1]
            ) / 2.0,

            "sqi": sqi,

            "snr": snr_db,

            "ac_rms": ac_rms,

            "cardiac_power": cardiac_power,

            "periodicity": periodicity,

            "classification": classification
        })

    return pd.DataFrame(
        records
    )


# ============================================================
# PEAK DETECTION
# ============================================================

def detect_candidate_peaks(
    filtered,
    fs
):
    """
    Candidate pulse peaks.

    Detection is performed independently on each finite segment.
    """

    candidate_indices = []

    finite = np.isfinite(
        filtered
    )

    labels, count = ndimage.label(
        finite
    )

    min_distance = int(
        MIN_PEAK_DISTANCE_SEC *
        fs
    )

    for label_id in range(
        1,
        count + 1
    ):

        idx = np.where(
            labels ==
            label_id
        )[0]

        if len(idx) < (
            2 * int(fs)
        ):
            continue

        segment = filtered[
            idx
        ]

        scale = robust_std(
            segment
        )

        if (
            not np.isfinite(scale) or
            scale <= 0
        ):
            continue

        prominence = (
            PEAK_PROMINENCE_FACTOR *
            scale
        )

        peaks, properties = signal.find_peaks(
            segment,
            distance=min_distance,
            prominence=prominence
        )

        candidate_indices.extend(
            idx[peaks].tolist()
        )

    return np.asarray(
        sorted(
            set(
                candidate_indices
            )
        ),
        dtype=int
    )


# ============================================================
# PEAK ACCEPTANCE
# ============================================================

def accept_peaks(
    candidate_indices,
    t,
    filtered
):
    """
    Classify candidate peaks.

    First apply physiological RR constraints.

    Then reject intervals that deviate too strongly from
    the local RR median.

    Returns:
        accepted_indices
        rejected_indices
    """

    candidate_indices = np.asarray(
        candidate_indices,
        dtype=int
    )

    if len(candidate_indices) < 2:

        return (
            np.asarray(
                [],
                dtype=int
            ),
            candidate_indices
        )

    peak_times = t[
        candidate_indices
    ]

    rr = np.diff(
        peak_times
    )

    candidate_valid = (
        (rr >= MIN_RR_SEC) &
        (rr <= MAX_RR_SEC)
    )

    accepted_interval = (
        candidate_valid.copy()
    )

    # Local RR median comparison.
    for i in range(
        len(rr)
    ):

        if not candidate_valid[i]:
            continue

        neighbours = []

        for j in (
            i - 2,
            i - 1,
            i + 1,
            i + 2
        ):

            if (
                0 <= j < len(rr) and
                candidate_valid[j]
            ):
                neighbours.append(
                    rr[j]
                )

        if len(neighbours) >= 2:

            local_median = np.median(
                neighbours
            )

            if local_median <= 0:
                continue

            deviation = (
                abs(
                    rr[i] -
                    local_median
                ) /
                local_median
            )

            if deviation > RR_LOCAL_DEVIATION:

                accepted_interval[i] = False

    # A peak belongs to accepted intervals only if both
    # adjacent intervals are acceptable.
    accepted_peak = np.ones(
        len(candidate_indices),
        dtype=bool
    )

    accepted_peak[0] = (
        accepted_interval[0]
    )

    accepted_peak[-1] = (
        accepted_interval[-1]
    )

    for i in range(
        1,
        len(candidate_indices) - 1
    ):

        accepted_peak[i] = (
            accepted_interval[i - 1] and
            accepted_interval[i]
        )

    accepted = candidate_indices[
        accepted_peak
    ]

    rejected = candidate_indices[
        ~accepted_peak
    ]

    return (
        accepted,
        rejected
    )


# ============================================================
# HR DATA
# ============================================================

def build_hr_dataframe(
    accepted_indices,
    t
):

    if len(accepted_indices) < 2:

        return pd.DataFrame(
            columns=[
                "time",
                "rr",
                "bpm",
                "bpm_trend"
            ]
        )

    peak_t = t[
        accepted_indices
    ]

    rr = np.diff(
        peak_t
    )

    bpm = (
        60.0 /
        rr
    )

    midpoint = (
        peak_t[:-1] +
        peak_t[1:]
    ) / 2.0

    valid = (
        np.isfinite(rr) &
        np.isfinite(bpm) &
        (rr > 0) &
        (bpm >= MIN_BPM) &
        (bpm <= MAX_BPM)
    )

    midpoint = midpoint[
        valid
    ]

    rr = rr[
        valid
    ]

    bpm = bpm[
        valid
    ]

    df = pd.DataFrame({
        "time": midpoint,
        "rr": rr,
        "bpm": bpm
    })

    # --------------------------------------------------------
    # Rolling time-based median trend.
    # --------------------------------------------------------

    if len(df) >= 3:

        trend = []

        half = (
            HR_TREND_SEC /
            2.0
        )

        times = df[
            "time"
        ].to_numpy()

        values = df[
            "bpm"
        ].to_numpy()

        for ti in times:

            mask = (
                np.abs(
                    times - ti
                ) <= half
            )

            if np.sum(mask) >= 2:

                trend.append(
                    np.median(
                        values[mask]
                    )
                )

            else:

                trend.append(
                    np.nan
                )

        df[
            "bpm_trend"
        ] = trend

    else:

        df[
            "bpm_trend"
        ] = np.nan

    return df


# ============================================================
# HRV
# ============================================================

def calculate_hrv(
    rr
):

    rr = np.asarray(
        rr,
        dtype=float
    )

    rr = rr[
        np.isfinite(rr)
    ]

    rr = rr[
        (rr >= MIN_RR_SEC) &
        (rr <= MAX_RR_SEC)
    ]

    if len(rr) < MIN_HRV_INTERVALS:
        return {}

    diff_rr = np.diff(
        rr
    )

    sdnn = (
        np.std(
            rr,
            ddof=1
        )
        if len(rr) > 1
        else np.nan
    )

    rmssd = (
        np.sqrt(
            np.mean(
                diff_rr ** 2
            )
        )
        if len(diff_rr)
        else np.nan
    )

    pnn50 = (
        100.0 *
        np.mean(
            np.abs(
                diff_rr
            ) > 0.050
        )
        if len(diff_rr)
        else np.nan
    )

    mean_nn = np.mean(
        rr
    )

    median_nn = np.median(
        rr
    )

    mean_hr = (
        60.0 /
        mean_nn
    )

    median_hr = (
        60.0 /
        median_nn
    )

    return {
        "n": len(rr),
        "mean_nn": mean_nn,
        "median_nn": median_nn,
        "mean_hr": mean_hr,
        "median_hr": median_hr,
        "sdnn": sdnn,
        "rmssd": rmssd,
        "pnn50": pnn50
    }


# ============================================================
# TIME FORMATTING
# ============================================================

def time_axis_kwargs():
    """
    Plotly time formatting.

    Example:
        00h00
        08h30
        14h30
        23h45
    """

    return {
        "type": "date",
        "tickformat": "%Hh%M",
        "hoverformat": "%H:%M:%S"
    }


def make_datetime_axis(
    elapsed_seconds,
    epoch
):

    return (
        pd.Timestamp(
            epoch,
            unit="ns"
        )
        +
        pd.to_timedelta(
            elapsed_seconds,
            unit="s"
        )
    )


# ============================================================
# PAGE 1
# SIGNAL QUALITY + DETECTED PEAKS
# ============================================================

def plot_signal_quality(
    t,
    raw,
    filtered,
    quality_df,
    candidate_indices,
    accepted_indices,
    rejected_indices,
    epoch,
    output
):

    dt = make_datetime_axis(
        t,
        epoch
    )

    # IMPORTANT:
    #
    # Both row 1 and row 4 genuinely have secondary Y axes.
    #
    # This is the correct way to define them.
    #
    # Do NOT manually assign yaxis="y8" or similar to traces.
    # Plotly handles the axis assignment through secondary_y.

    fig = make_subplots(
        rows=4,
        cols=1,
        shared_xaxes=True,
        vertical_spacing=0.055,
        row_heights=[
            0.42,
            0.20,
            0.19,
            0.19
        ],
        specs=[
            [
                {
                    "secondary_y": True
                }
            ],
            [
                {
                    "secondary_y": False
                }
            ],
            [
                {
                    "secondary_y": False
                }
            ],
            [
                {
                    "secondary_y": True
                }
            ]
        ],
        subplot_titles=(
            "Raw and filtered pulse signal",
            "Signal Quality Index (SQI)",
            "Signal-to-Noise Ratio (SNR)",
            "Signal level and waveform metrics"
        )
    )

    # ========================================================
    # RAW SIGNAL
    # ========================================================

    finite_raw = np.isfinite(
        raw
    )

    fig.add_trace(
        go.Scattergl(
            x=dt[
                finite_raw
            ],
            y=raw[
                finite_raw
            ],
            mode="lines",
            name="Raw signal",
            line=dict(
                width=1
            ),
            opacity=0.55,
            hovertemplate=(
                "%{x|%H:%M:%S}<br>"
                "Raw: %{y:.2f}"
                "<extra></extra>"
            )
        ),
        row=1,
        col=1,
        secondary_y=False
    )

    # ========================================================
    # FILTERED SIGNAL
    # ========================================================

    finite_filtered = np.isfinite(
        filtered
    )

    fig.add_trace(
        go.Scattergl(
            x=dt[
                finite_filtered
            ],
            y=filtered[
                finite_filtered
            ],
            mode="lines",
            name="Filtered signal",
            line=dict(
                width=1.5
            ),
            hovertemplate=(
                "%{x|%H:%M:%S}<br>"
                "Filtered: %{y:.2f}"
                "<extra></extra>"
            )
        ),
        row=1,
        col=1,
        secondary_y=True
    )

    # ========================================================
    # ACCEPTED PEAKS
    #
    # Pure blue crosses.
    # No stroke.
    # ========================================================

    if len(accepted_indices):

        ai = accepted_indices

        fig.add_trace(
            go.Scattergl(
                x=dt[ai],
                y=filtered[ai],
                mode="markers",
                name="Accepted pulse",
                marker=dict(
                    symbol="x",
                    size=10,
                    color="blue",
                    line=dict(
                        color="blue",
                        width=0
                    )
                ),
                hovertemplate=(
                    "%{x|%H:%M:%S}<br>"
                    "Accepted pulse<br>"
                    "Signal: %{y:.2f}"
                    "<extra></extra>"
                )
            ),
            row=1,
            col=1,
            secondary_y=True
        )

    # ========================================================
    # REJECTED PEAKS
    #
    # Pure red crosses.
    # No stroke.
    # ========================================================

    if len(rejected_indices):

        ri = rejected_indices

        fig.add_trace(
            go.Scattergl(
                x=dt[ri],
                y=filtered[ri],
                mode="markers",
                name="Rejected pulse",
                marker=dict(
                    symbol="x",
                    size=10,
                    color="red",
                    line=dict(
                        color="red",
                        width=0
                    )
                ),
                hovertemplate=(
                    "%{x|%H:%M:%S}<br>"
                    "Rejected pulse<br>"
                    "Signal: %{y:.2f}"
                    "<extra></extra>"
                )
            ),
            row=1,
            col=1,
            secondary_y=True
        )

    # ========================================================
    # QUALITY WINDOWS
    # ========================================================

    if len(quality_df):

        qt = make_datetime_axis(
            quality_df[
                "time"
            ].to_numpy(),
            epoch
        )

        sqi = quality_df[
            "sqi"
        ].to_numpy(
            dtype=float
        )

        # ----------------------------------------------------
        # SQI trace
        # ----------------------------------------------------

        fig.add_trace(
            go.Scatter(
                x=qt,
                y=sqi,
                mode="lines+markers",
                name="SQI",
                line=dict(
                    width=2
                ),
                marker=dict(
                    size=5
                ),
                customdata=np.column_stack([
                    quality_df[
                        "classification"
                    ].to_numpy(),

                    quality_df[
                        "snr"
                    ].to_numpy(),

                    quality_df[
                        "ac_rms"
                    ].to_numpy(),

                    quality_df[
                        "cardiac_power"
                    ].to_numpy(),

                    quality_df[
                        "periodicity"
                    ].to_numpy()
                ]),
                hovertemplate=(
                    "%{x|%H:%M:%S}<br>"
                    "<b>SQI: %{y:.3f}</b><br>"
                    "Classification: %{customdata[0]}<br>"
                    "SNR: %{customdata[1]:.2f} dB<br>"
                    "AC RMS: %{customdata[2]:.2f}<br>"
                    "Cardiac power: %{customdata[3]:.3f}<br>"
                    "Periodicity: %{customdata[4]:.3f}"
                    "<extra></extra>"
                )
            ),
            row=2,
            col=1
        )

        # ----------------------------------------------------
        # SQI background zones
        #
        # 0.60 -> GOOD
        # 0.35 -> QUESTIONABLE
        # below -> BAD
        # ----------------------------------------------------

        fig.add_hrect(
            y0=SQI_GOOD,
            y1=1.0,
            fillcolor="rgba(0, 180, 0, 0.08)",
            line_width=0,
            row=2,
            col=1
        )

        fig.add_hrect(
            y0=SQI_QUESTIONABLE,
            y1=SQI_GOOD,
            fillcolor="rgba(255, 180, 0, 0.08)",
            line_width=0,
            row=2,
            col=1
        )

        fig.add_hrect(
            y0=0.0,
            y1=SQI_QUESTIONABLE,
            fillcolor="rgba(255, 0, 0, 0.06)",
            line_width=0,
            row=2,
            col=1
        )

        # ----------------------------------------------------
        # EXACT SQI THRESHOLD LINES
        # ----------------------------------------------------

        fig.add_hline(
            y=SQI_GOOD,
            line_dash="dash",
            line_width=2,
            annotation_text=(
                f"GOOD ≥ {SQI_GOOD:.2f}"
            ),
            annotation_position="top left",
            row=2,
            col=1
        )

        fig.add_hline(
            y=SQI_QUESTIONABLE,
            line_dash="dot",
            line_width=2,
            annotation_text=(
                f"QUESTIONABLE ≥ "
                f"{SQI_QUESTIONABLE:.2f}"
            ),
            annotation_position="bottom left",
            row=2,
            col=1
        )

        # ----------------------------------------------------
        # SNR
        # ----------------------------------------------------

        fig.add_trace(
            go.Scatter(
                x=qt,
                y=quality_df[
                    "snr"
                ],
                mode="lines+markers",
                name="SNR",
                line=dict(
                    width=2
                ),
                hovertemplate=(
                    "%{x|%H:%M:%S}<br>"
                    "SNR: %{y:.2f} dB"
                    "<extra></extra>"
                )
            ),
            row=3,
            col=1
        )

        fig.add_hline(
            y=SNR_GOOD_DB,
            line_dash="dash",
            annotation_text=(
                f"Good SNR reference "
                f"{SNR_GOOD_DB:.1f} dB"
            ),
            annotation_position="top left",
            row=3,
            col=1
        )

        fig.add_hline(
            y=SNR_QUESTIONABLE_DB,
            line_dash="dot",
            annotation_text="0 dB",
            annotation_position="bottom left",
            row=3,
            col=1
        )

        # ----------------------------------------------------
        # AC RMS
        # ----------------------------------------------------

        fig.add_trace(
            go.Scatter(
                x=qt,
                y=quality_df[
                    "ac_rms"
                ],
                mode="lines+markers",
                name="AC RMS",
                line=dict(
                    width=2
                ),
                hovertemplate=(
                    "%{x|%H:%M:%S}<br>"
                    "AC RMS: %{y:.2f}"
                    "<extra></extra>"
                )
            ),
            row=4,
            col=1,
            secondary_y=False
        )

        # ----------------------------------------------------
        # CARDIAC POWER
        #
        # IMPORTANT:
        # No explicit yaxis="y8".
        #
        # secondary_y=True is sufficient.
        # ----------------------------------------------------

        fig.add_trace(
            go.Scatter(
                x=qt,
                y=quality_df[
                    "cardiac_power"
                ],
                mode="lines",
                name="Cardiac power",
                line=dict(
                    width=2
                ),
                hovertemplate=(
                    "%{x|%H:%M:%S}<br>"
                    "Cardiac power: %{y:.3f}"
                    "<extra></extra>"
                )
            ),
            row=4,
            col=1,
            secondary_y=True
        )

    # ========================================================
    # AXES
    # ========================================================

    fig.update_yaxes(
        title_text="Raw signal",
        row=1,
        col=1,
        secondary_y=False
    )

    fig.update_yaxes(
        title_text="Filtered signal",
        row=1,
        col=1,
        secondary_y=True
    )

    fig.update_yaxes(
        title_text="SQI",
        range=[0, 1],
        row=2,
        col=1
    )

    fig.update_yaxes(
        title_text="SNR (dB)",
        row=3,
        col=1
    )

    fig.update_yaxes(
        title_text="AC RMS",
        row=4,
        col=1,
        secondary_y=False
    )

    fig.update_yaxes(
        title_text="Cardiac power",
        range=[0, 1],
        row=4,
        col=1,
        secondary_y=True
    )

    for row in range(
        1,
        5
    ):

        fig.update_xaxes(
            row=row,
            col=1,
            **time_axis_kwargs()
        )

    fig.update_layout(
        title=(
            "Pulse Signal Quality, "
            "Filtering and Pulse Detection"
        ),
        height=1250,
        hovermode="x unified",
        legend=dict(
            orientation="h",
            yanchor="bottom",
            y=1.02,
            xanchor="left",
            x=0
        ),
        margin=dict(
            l=90,
            r=90,
            t=120,
            b=70
        )
    )

    fig.write_html(
        output,
        include_plotlyjs=True
    )

    return fig


# ============================================================
# PAGE 2
# HEART RATE + DISTRIBUTION
# ============================================================

def plot_heart_rate(
    bpm_df,
    epoch,
    output
):

    if len(bpm_df) == 0:

        print(
            "No BPM data available."
        )

        return None

    plot_df = bpm_df.copy()

    plot_df["bpm"] = pd.to_numeric(
        plot_df["bpm"],
        errors="coerce"
    )

    plot_df["bpm_trend"] = pd.to_numeric(
        plot_df["bpm_trend"],
        errors="coerce"
    )

    # Remove invalid BPM values.
    plot_df = plot_df[
        np.isfinite(
            plot_df[
                "bpm"
            ].to_numpy(
                dtype=float
            )
        )
    ].copy()

    if len(plot_df) == 0:

        print(
            "No finite BPM values."
        )

        return None

    t = make_datetime_axis(
        plot_df[
            "time"
        ].to_numpy(),
        epoch
    )

    bpm = plot_df[
        "bpm"
    ].to_numpy(
        dtype=float
    )

    trend = plot_df[
        "bpm_trend"
    ].to_numpy(
        dtype=float
    )

    bpm_min = np.min(
        bpm
    )

    bpm_max = np.max(
        bpm
    )

    if bpm_max == bpm_min:

        margin = 5.0

    else:

        margin = max(
            2.0,
            0.08 *
            (
                bpm_max -
                bpm_min
            )
        )

    y_min = max(
        MIN_BPM - 5,
        bpm_min - margin
    )

    y_max = min(
        MAX_BPM + 5,
        bpm_max + margin
    )

    mean_bpm = np.mean(
        bpm
    )

    median_bpm = np.median(
        bpm
    )

    std_bpm = (
        np.std(
            bpm,
            ddof=1
        )
        if len(bpm) > 1
        else 0.0
    )

    # ========================================================
    # Fine histogram
    #
    # 1 BPM bins.
    # ========================================================

    hist_start = np.floor(
        bpm_min
    )

    hist_end = np.ceil(
        bpm_max
    )

    if hist_end <= hist_start:
        hist_end = (
            hist_start + 1
        )

    bin_size = 1.0

    bin_edges = np.arange(
        hist_start,
        hist_end +
        bin_size,
        bin_size
    )

    # Guarantee at least two edges.
    if len(bin_edges) < 2:

        bin_edges = np.array([
            hist_start,
            hist_start + bin_size
        ])

    counts, edges = np.histogram(
        bpm,
        bins=bin_edges
    )

    centers = (
        edges[:-1] +
        edges[1:]
    ) / 2.0

    # ========================================================
    # Figure
    # ========================================================

    fig = make_subplots(
        rows=2,
        cols=1,
        vertical_spacing=0.14,
        row_heights=[
            0.62,
            0.38
        ],
        subplot_titles=(
            "Heart Rate vs Time",
            "Heart Rate Distribution"
        )
    )

    # ========================================================
    # Instantaneous HR
    # ========================================================

    fig.add_trace(
        go.Scattergl(
            x=t,
            y=bpm,
            mode="markers",
            name="Instantaneous HR",
            marker=dict(
                size=6
            ),
            customdata=(
                plot_df[
                    "rr"
                ].to_numpy(
                    dtype=float
                ) * 1000.0
            ),
            hovertemplate=(
                "%{x|%H:%M:%S}<br>"
                "<b>%{y:.1f} BPM</b><br>"
                "NN: %{customdata:.0f} ms"
                "<extra></extra>"
            )
        ),
        row=1,
        col=1
    )

    # ========================================================
    # HR trend
    # ========================================================

    valid_trend = np.isfinite(
        trend
    )

    if np.any(
        valid_trend
    ):

        fig.add_trace(
            go.Scatter(
                x=t[
                    valid_trend
                ],
                y=trend[
                    valid_trend
                ],
                mode="lines",
                name=(
                    f"{HR_TREND_SEC:.0f} s "
                    "median trend"
                ),
                line=dict(
                    width=3
                ),
                hovertemplate=(
                    "%{x|%H:%M:%S}<br>"
                    "Trend: %{y:.1f} BPM"
                    "<extra></extra>"
                )
            ),
            row=1,
            col=1
        )

    # Mean HR reference.
    fig.add_hline(
        y=mean_bpm,
        line_dash="dot",
        annotation_text=(
            f"Mean {mean_bpm:.1f} BPM"
        ),
        annotation_position="top left",
        row=1,
        col=1
    )

    fig.update_yaxes(
        title_text="Heart rate (BPM)",
        range=[
            y_min,
            y_max
        ],
        row=1,
        col=1
    )

    fig.update_xaxes(
        title_text="Time",
        row=1,
        col=1,
        **time_axis_kwargs()
    )

    # ========================================================
    # HISTOGRAM
    #
    # Explicit Bar trace.
    #
    # X = BPM
    # Y = number of NN intervals
    # ========================================================

    fig.add_trace(
        go.Bar(
            x=centers,
            y=counts,
            width=0.90,
            name="NN interval count",
            hovertemplate=(
                "HR: %{x:.0f} BPM<br>"
                "Intervals: %{y}<extra></extra>"
            )
        ),
        row=2,
        col=1
    )

    # ========================================================
    # HISTOGRAM AXIS
    # ========================================================

    hist_y_max = (
        int(np.max(counts))
        if len(counts) and np.max(counts) > 0
        else 1
    )

    # Add a little headroom for annotations.
    hist_y_top = max(
        hist_y_max * 1.18,
        1.0
    )

    fig.update_xaxes(
        title_text="Heart rate (BPM)",
        range=[
            hist_start - 1,
            hist_end + 1
        ],
        dtick=5,
        tickmode="linear",
        type="linear",
        showgrid=True,
        row=2,
        col=1
    )

    fig.update_yaxes(
        title_text="Number of NN intervals",
        range=[
            0,
            hist_y_top
        ],
        rangemode="tozero",
        showgrid=True,
        row=2,
        col=1
    )

    # ========================================================
    # MEAN / MEDIAN REFERENCE LINES
    #
    # Use add_vline with the correct subplot.
    # This avoids the previous yref="paper" problem.
    # ========================================================

    fig.add_vline(
        x=median_bpm,
        line_dash="dash",
        line_width=2,
        row=2,
        col=1
    )

    fig.add_vline(
        x=mean_bpm,
        line_dash="dot",
        line_width=2,
        row=2,
        col=1
    )

    # ========================================================
    # MEAN / MEDIAN LABELS
    #
    # Put them inside the histogram's own Y coordinate system.
    #
    # If they are close together, use one combined label.
    # ========================================================

    if abs(
        mean_bpm -
        median_bpm
    ) < 1.0:

        combined_x = (
            mean_bpm +
            median_bpm
        ) / 2.0

        fig.add_annotation(
            x=combined_x,
            y=hist_y_top * 0.94,
            xref="x2",
            yref="y2",
            text=(
                f"Mean {mean_bpm:.1f} / "
                f"Median {median_bpm:.1f} BPM"
            ),
            showarrow=False,
            xanchor="center",
            yanchor="top",
            bgcolor="rgba(255,255,255,0.80)",
            borderwidth=1,
            bordercolor="gray"
        )

    else:

        fig.add_annotation(
            x=median_bpm,
            y=hist_y_top * 0.92,
            xref="x2",
            yref="y2",
            text=(
                f"Median "
                f"{median_bpm:.1f} BPM"
            ),
            showarrow=False,
            xanchor="left",
            yanchor="top",
            bgcolor="rgba(255,255,255,0.80)",
            borderwidth=1,
            bordercolor="gray"
        )

        fig.add_annotation(
            x=mean_bpm,
            y=hist_y_top * 0.72,
            xref="x2",
            yref="y2",
            text=(
                f"Mean "
                f"{mean_bpm:.1f} BPM"
            ),
            showarrow=False,
            xanchor="left",
            yanchor="top",
            bgcolor="rgba(255,255,255,0.80)",
            borderwidth=1,
            bordercolor="gray"
        )

    # ========================================================
    # LAYOUT
    # ========================================================

    fig.update_layout(
        title="Heart Rate Analysis",
        height=850,
        hovermode="x unified",
        bargap=0.05,
        legend=dict(
            orientation="h",
            yanchor="bottom",
            y=1.02,
            xanchor="left",
            x=0
        ),
        margin=dict(
            l=90,
            r=50,
            t=120,
            b=75
        )
    )

    fig.write_html(
        output,
        include_plotlyjs=True
    )

    print()
    print(
        f"Heart-rate plot: "
        f"{len(bpm)} valid intervals"
    )

    print(
        f"HR range: "
        f"{bpm_min:.1f}–{bpm_max:.1f} BPM"
    )

    print(
        f"HR mean: "
        f"{mean_bpm:.1f} BPM"
    )

    print(
        f"HR median: "
        f"{median_bpm:.1f} BPM"
    )

    print(
        f"HR std: "
        f"{std_bpm:.1f} BPM"
    )

    return fig


# ============================================================
# HRV PAGE
# ============================================================

def plot_hrv(
    bpm_df,
    epoch,
    output
):

    if len(bpm_df) < MIN_HRV_INTERVALS:

        print(
            "Not enough NN intervals for HRV plot."
        )

        return None

    rr = bpm_df[
        "rr"
    ].to_numpy(
        dtype=float
    )

    time = bpm_df[
        "time"
    ].to_numpy(
        dtype=float
    )

    valid = (
        np.isfinite(rr) &
        (rr >= MIN_RR_SEC) &
        (rr <= MAX_RR_SEC)
    )

    rr = rr[
        valid
    ]

    time = time[
        valid
    ]

    if len(rr) < MIN_HRV_INTERVALS:
        return None

    # ========================================================
    # Time-domain HRV
    # ========================================================

    diff_rr = np.diff(
        rr
    )

    sdnn = (
        np.std(
            rr,
            ddof=1
        )
        if len(rr) > 1
        else np.nan
    )

    rmssd = np.sqrt(
        np.mean(
            diff_rr ** 2
        )
    )

    pnn50 = (
        100.0 *
        np.mean(
            np.abs(
                diff_rr
            ) > 0.050
        )
    )

    mean_nn = np.mean(
        rr
    )

    median_nn = np.median(
        rr
    )

    mean_hr = (
        60.0 /
        mean_nn
    )

    median_hr = (
        60.0 /
        median_nn
    )

    # ========================================================
    # Poincaré
    # ========================================================

    rr1 = rr[:-1]
    rr2 = rr[1:]

    # ========================================================
    # Figure
    # ========================================================

    fig = make_subplots(
        rows=3,
        cols=1,
        vertical_spacing=0.12,
        row_heights=[
            0.40,
            0.30,
            0.30
        ],
        subplot_titles=(
            "NN interval over time",
            "NN interval distribution",
            "Poincaré plot"
        )
    )

    dt = make_datetime_axis(
        time,
        epoch
    )

    # ========================================================
    # NN OVER TIME
    # ========================================================

    fig.add_trace(
        go.Scatter(
            x=dt,
            y=rr * 1000.0,
            mode="lines+markers",
            name="NN interval",
            marker=dict(
                size=5
            ),
            hovertemplate=(
                "%{x|%H:%M:%S}<br>"
                "NN: %{y:.1f} ms"
                "<extra></extra>"
            )
        ),
        row=1,
        col=1
    )

    fig.update_xaxes(
        title_text="Time",
        row=1,
        col=1,
        **time_axis_kwargs()
    )

    fig.update_yaxes(
        title_text="NN interval (ms)",
        row=1,
        col=1
    )

    # ========================================================
    # NN HISTOGRAM
    # ========================================================

    rr_ms = rr * 1000.0

    rr_min = np.floor(
        np.min(rr_ms)
    )

    rr_max = np.ceil(
        np.max(rr_ms)
    )

    bin_size = 10.0

    edges = np.arange(
        rr_min,
        rr_max + bin_size,
        bin_size
    )

    if len(edges) < 2:

        edges = np.array([
            rr_min,
            rr_min + bin_size
        ])

    counts, edges = np.histogram(
        rr_ms,
        bins=edges
    )

    centers = (
        edges[:-1] +
        edges[1:]
    ) / 2.0

    fig.add_trace(
        go.Bar(
            x=centers,
            y=counts,
            width=9.0,
            name="NN count",
            hovertemplate=(
                "NN: %{x:.0f} ms<br>"
                "Intervals: %{y}"
                "<extra></extra>"
            )
        ),
        row=2,
        col=1
    )

    fig.update_xaxes(
        title_text="NN interval (ms)",
        row=2,
        col=1
    )

    fig.update_yaxes(
        title_text="Number of intervals",
        rangemode="tozero",
        row=2,
        col=1
    )

    # ========================================================
    # POINCARÉ
    # ========================================================

    fig.add_trace(
        go.Scatter(
            x=rr1 * 1000.0,
            y=rr2 * 1000.0,
            mode="markers",
            name="NN pairs",
            marker=dict(
                size=7
            ),
            hovertemplate=(
                "NN(n): %{x:.1f} ms<br>"
                "NN(n+1): %{y:.1f} ms"
                "<extra></extra>"
            )
        ),
        row=3,
        col=1
    )

    min_p = min(
        np.min(rr1),
        np.min(rr2)
    ) * 1000.0

    max_p = max(
        np.max(rr1),
        np.max(rr2)
    ) * 1000.0

    fig.add_shape(
        type="line",
        x0=min_p,
        y0=min_p,
        x1=max_p,
        y1=max_p,
        line=dict(
            dash="dash"
        ),
        row=3,
        col=1
    )

    fig.update_xaxes(
        title_text="NN(n) (ms)",
        row=3,
        col=1
    )

    fig.update_yaxes(
        title_text="NN(n+1) (ms)",
        row=3,
        col=1
    )

    # ========================================================
    # HRV STATISTICS
    # ========================================================

    fig.add_annotation(
        x=0.99,
        y=0.99,
        xref="paper",
        yref="paper",
        xanchor="right",
        yanchor="top",
        align="left",
        showarrow=False,
        text=(
            f"<b>HRV summary</b><br>"
            f"Mean HR: {mean_hr:.1f} BPM<br>"
            f"Median HR: {median_hr:.1f} BPM<br>"
            f"Mean NN: {mean_nn * 1000:.1f} ms<br>"
            f"Median NN: {median_nn * 1000:.1f} ms<br>"
            f"SDNN: {sdnn * 1000:.1f} ms<br>"
            f"RMSSD: {rmssd * 1000:.1f} ms<br>"
            f"pNN50: {pnn50:.1f}%<br>"
            f"N: {len(rr)}"
        ),
        bgcolor="rgba(255,255,255,0.85)",
        bordercolor="gray",
        borderwidth=1
    )

    fig.update_layout(
        title="Heart Rate Variability",
        height=1050,
        hovermode="closest",
        margin=dict(
            l=90,
            r=90,
            t=120,
            b=70
        )
    )

    fig.write_html(
        output,
        include_plotlyjs=True
    )

    return fig


# ============================================================
# SUMMARY
# ============================================================

def print_quality_summary(
    quality_df
):

    if len(quality_df) == 0:

        print(
            "\nNo usable quality windows."
        )

        return

    sqi = quality_df[
        "sqi"
    ].to_numpy(
        dtype=float
    )

    snr = quality_df[
        "snr"
    ].to_numpy(
        dtype=float
    )

    ac_rms = quality_df[
        "ac_rms"
    ].to_numpy(
        dtype=float
    )

    cardiac_power = quality_df[
        "cardiac_power"
    ].to_numpy(
        dtype=float
    )

    periodicity = quality_df[
        "periodicity"
    ].to_numpy(
        dtype=float
    )

    classification = (
        quality_df[
            "classification"
        ]
    )

    good = (
        classification ==
        "GOOD"
    )

    questionable = (
        classification ==
        "QUESTIONABLE"
    )

    bad = (
        classification ==
        "BAD"
    )

    print()

    print(
        f"Median SQI           : "
        f"{np.nanmedian(sqi):.3f}"
    )

    print(
        f"GOOD windows         : "
        f"{100.0 * np.mean(good):.1f}%"
    )

    print(
        f"QUESTIONABLE windows : "
        f"{100.0 * np.mean(questionable):.1f}%"
    )

    print(
        f"BAD windows          : "
        f"{100.0 * np.mean(bad):.1f}%"
    )

    print(
        f"Median SNR           : "
        f"{np.nanmedian(snr):.2f} dB"
    )

    print(
        f"Minimum SNR          : "
        f"{np.nanmin(snr):.2f} dB"
    )

    print(
        f"Maximum SNR          : "
        f"{np.nanmax(snr):.2f} dB"
    )

    print(
        f"Median AC RMS        : "
        f"{np.nanmedian(ac_rms):.2f}"
    )

    print(
        f"Median cardiac power: "
        f"{np.nanmedian(cardiac_power):.3f}"
    )

    print(
        f"Median periodicity   : "
        f"{np.nanmedian(periodicity):.3f}"
    )


# ============================================================
# DOMINANT SPECTRAL HR
# ============================================================

def dominant_spectral_hr(
    filtered,
    fs
):

    x = filtered[
        np.isfinite(filtered)
    ]

    if len(x) < (
        5 * fs
    ):

        return np.nan

    x = signal.detrend(
        x
    )

    freqs, psd = signal.periodogram(
        x,
        fs=fs,
        window="hann"
    )

    mask = (
        (freqs >= MIN_BPM / 60.0) &
        (freqs <= MAX_BPM / 60.0)
    )

    if not np.any(mask):
        return np.nan

    selected_freqs = freqs[
        mask
    ]

    selected_psd = psd[
        mask
    ]

    peak_idx = np.argmax(
        selected_psd
    )

    return (
        selected_freqs[
            peak_idx
        ] * 60.0
    )


# ============================================================
# MAIN
# ============================================================

def main():

    parser = argparse.ArgumentParser(
        description=(
            "PPG pulse / BPM / HRV "
            "analysis"
        )
    )

    parser.add_argument(
        "csv_file",
        type=Path,
        help="CSV file"
    )

    parser.add_argument(
        "--fs",
        type=float,
        default=DEFAULT_FS,
        help=(
            "Nominal MCU sampling "
            "frequency"
        )
    )

    args = parser.parse_args()

    # ========================================================
    # LOAD
    # ========================================================

    df = load_csv(
        args.csv_file
    )

    if len(df) < 2:

        raise RuntimeError(
            "Not enough samples."
        )

    # ========================================================
    # TIMESTAMP ANALYSIS
    # ========================================================

    ts_info = timestamp_analysis(
        df,
        args.fs
    )

    ts_ns = ts_info[
        "timestamps_ns"
    ]

    raw_original = df[
        "Data"
    ].to_numpy(
        dtype=float
    )

    # ========================================================
    # UNIFORM RESAMPLING
    # ========================================================

    t, raw = resample_to_uniform(
        ts_ns,
        raw_original,
        args.fs
    )

    # ========================================================
    # REMOVE DC / BASELINE BEFORE FILTERING
    #
    # Detrending is performed separately on each finite
    # segment so acquisition gaps do not couple different
    # portions of the signal.
    # ========================================================

    raw_for_filter = raw.copy()

    finite = np.isfinite(
        raw_for_filter
    )

    labels, count = ndimage.label(
        finite
    )

    for label_id in range(
        1,
        count + 1
    ):

        idx = np.where(
            labels ==
            label_id
        )[0]

        if len(idx) < 3:
            continue

        segment = raw_for_filter[
            idx
        ]

        try:

            raw_for_filter[
                idx
            ] = signal.detrend(
                segment,
                type="linear"
            )

        except ValueError:

            # Keep original segment if detrending fails.
            pass

    # ========================================================
    # BAND-PASS
    # ========================================================

    filtered = butter_bandpass(
        raw_for_filter,
        args.fs,
        FILTER_LOW_HZ,
        FILTER_HIGH_HZ,
        FILTER_ORDER
    )

    # ========================================================
    # QUALITY
    # ========================================================

    quality_df = analyze_quality(
        t,
        raw,
        filtered,
        args.fs
    )

    print_quality_summary(
        quality_df
    )

    # ========================================================
    # DOMINANT SPECTRAL HR
    # ========================================================

    spectral_hr = dominant_spectral_hr(
        filtered,
        args.fs
    )

    if np.isfinite(
        spectral_hr
    ):

        print()

        print(
            f"Dominant spectral HR : "
            f"{spectral_hr:.1f} BPM"
        )

    # ========================================================
    # PEAK DETECTION
    # ========================================================

    candidate_indices = detect_candidate_peaks(
        filtered,
        args.fs
    )

    (
        accepted_indices,
        rejected_indices
    ) = accept_peaks(
        candidate_indices,
        t,
        filtered
    )

    print()

    print(
        f"Candidate peaks      : "
        f"{len(candidate_indices)}"
    )

    print(
        f"Accepted peaks       : "
        f"{len(accepted_indices)}"
    )

    print(
        f"Rejected candidates  : "
        f"{len(rejected_indices)}"
    )

    # ========================================================
    # HR
    # ========================================================

    bpm_df = build_hr_dataframe(
        accepted_indices,
        t
    )

    print(
        f"NN intervals         : "
        f"{len(bpm_df)}"
    )

    if len(bpm_df):

        rr = bpm_df[
            "rr"
        ].to_numpy(
            dtype=float
        )

        median_nn = np.median(
            rr
        )

        mean_nn = np.mean(
            rr
        )

        median_hr = (
            60.0 /
            median_nn
        )

        mean_hr = (
            60.0 /
            mean_nn
        )

        print(
            f"Median NN            : "
            f"{median_nn * 1000:.1f} ms"
        )

        print(
            f"Median HR            : "
            f"{median_hr:.1f} BPM"
        )

        print(
            f"Mean HR from NN      : "
            f"{mean_hr:.1f} BPM"
        )

    # ========================================================
    # HRV
    # ========================================================

    if len(bpm_df):

        hrv = calculate_hrv(
            bpm_df[
                "rr"
            ].to_numpy(
                dtype=float
            )
        )

    else:

        hrv = {}

    if hrv:

        print()

        print(
            f"Mean HR     : "
            f"{hrv['mean_hr']:.1f} BPM"
        )

        print(
            f"Mean NN     : "
            f"{hrv['mean_nn'] * 1000:.1f} ms"
        )

        print(
            f"SDNN        : "
            f"{hrv['sdnn'] * 1000:.1f} ms"
        )

        print(
            f"RMSSD       : "
            f"{hrv['rmssd'] * 1000:.1f} ms"
        )

        print(
            f"pNN50       : "
            f"{hrv['pnn50']:.1f} %"
        )

    else:

        print()

        print(
            "Not enough valid NN intervals "
            "for HRV."
        )

    # ========================================================
    # OUTPUT FILES
    # ========================================================

    output_dir = (
        args.csv_file.parent
    )

    output_signal = (
        output_dir /
        "01_signal_quality.html"
    )

    output_hr = (
        output_dir /
        "02_heart_rate.html"
    )

    output_hrv = (
        output_dir /
        "03_hrv.html"
    )

    # ========================================================
    # PLOT 1
    # ========================================================

    plot_signal_quality(
        t,
        raw,
        filtered,
        quality_df,
        candidate_indices,
        accepted_indices,
        rejected_indices,
        ts_ns[0],
        output_signal
    )

    # ========================================================
    # PLOT 2
    # ========================================================

    plot_heart_rate(
        bpm_df,
        ts_ns[0],
        output_hr
    )

    # ========================================================
    # PLOT 3
    # ========================================================

    plot_hrv(
        bpm_df,
        ts_ns[0],
        output_hrv
    )

    # ========================================================
    # OUTPUT
    # ========================================================

    print()

    print("=" * 60)
    print("OUTPUT")
    print("=" * 60)

    print(
        f"Signal quality : "
        f"{output_signal}"
    )

    print(
        f"Heart rate     : "
        f"{output_hr}"
    )

    print(
        f"HRV            : "
        f"{output_hrv}"
    )

    print()


# ============================================================
# ENTRY POINT
# ============================================================

if __name__ == "__main__":
    main()
