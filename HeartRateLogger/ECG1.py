#!/usr/bin/env python3

import argparse
from pathlib import Path
import numpy as np
import pandas as pd


def load_csv(path: Path):
    """
    Load CSV produced by the new logger.

    Expected format:
        HostTime_ns,Data
        1754774325123456789,640
        1754774325128456790,638
        ...
    """
    df = pd.read_csv(path, usecols=["HostTime_ns", "Data"])

    # Ensure integer nanoseconds
    df["HostTime_ns"] = pd.to_numeric(df["HostTime_ns"], errors="coerce")
    df = df.dropna(subset=["HostTime_ns"])

    return df


def sanity_check(df: pd.DataFrame, nominal_fs: float = 190.0):
    ts_ns = df["HostTime_ns"].to_numpy(dtype=np.int64)

    if len(ts_ns) < 2:
        print("Not enough samples.")
        return

    dt_ns = np.diff(ts_ns)
    dt = dt_ns / 1e9          # seconds

    duration = (ts_ns[-1] - ts_ns[0]) / 1e9
    n = len(ts_ns)

    estimated_fs = (n - 1) / duration if duration > 0 else float("nan")

    median_dt = np.median(dt)
    mean_dt = np.mean(dt)
    std_dt = np.std(dt)

    expected_dt = 1.0 / nominal_fs

    duplicate = int(np.sum(dt_ns == 0))
    large_gaps = dt[dt > 1.5 * expected_dt]
    missing_est = int(np.round((large_gaps / expected_dt) - 1).clip(min=0).sum())

    jitter_rms = np.sqrt(np.mean((dt - expected_dt) ** 2))

    print("=" * 60)
    print("RECORDING SUMMARY")
    print("=" * 60)
    print(f"Samples                : {n}")
    print(f"Duration               : {duration:.3f} s ({duration/60:.2f} min)")
    print(f"Nominal sample rate    : {nominal_fs:.2f} Hz")
    print(f"Estimated sample rate  : {estimated_fs:.3f} Hz")
    print()

    print("TIMESTAMP INTERVAL STATISTICS")
    print("-" * 60)
    print(f"Mean interval          : {mean_dt*1000:.6f} ms")
    print(f"Median interval        : {median_dt*1000:.6f} ms")
    print(f"Std deviation          : {std_dt*1000:.6f} ms")
    print(f"RMS jitter             : {jitter_rms*1000:.6f} ms")
    print(f"Min interval           : {dt.min()*1000:.6f} ms")
    print(f"Max interval           : {dt.max()*1000:.6f} ms")
    print()

    print("DATA QUALITY")
    print("-" * 60)
    print(f"Duplicate timestamps   : {duplicate}")
    print(f"Large gaps (>1.5x dt)  : {len(large_gaps)}")
    print(f"Estimated missing samp.: {missing_est}")

    fs_error = abs(estimated_fs - nominal_fs) / nominal_fs * 100
    if fs_error > 2:
        print(f"\nWARNING: Sample rate differs from nominal by {fs_error:.2f}%")
    else:
        print(f"\nSample rate within ±2% of nominal.")

    print()
    print("INTERVAL HISTOGRAM (0.1 ms bins)")
    print("-" * 60)

    # 0.1 ms bins are more meaningful with nanosecond timestamps
    hist = pd.Series(np.round(dt * 10000) / 10).value_counts().sort_index()

    for interval_ms, count in hist.items():
        if count > 0:
            print(f"{interval_ms:6.1f} ms : {count:>8}")


def main():
    parser = argparse.ArgumentParser(description="Heartbeat pulse sensor analysis")
    parser.add_argument("csv_file", type=Path, help="CSV file in local folder")
    parser.add_argument("--fs", type=float, default=190.0,
                        help="Nominal sample rate")
    args = parser.parse_args()

    df = load_csv(args.csv_file)
    sanity_check(df, args.fs)


if __name__ == "__main__":
    main()
