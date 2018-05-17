[1mdiff --git a/Eclipse/m32_uart_buffer/.cproject b/Eclipse/m32_uart_buffer/.cproject[m
[1mindex 29424d9..56dfc8c 100644[m
[1m--- a/Eclipse/m32_uart_buffer/.cproject[m
[1m+++ b/Eclipse/m32_uart_buffer/.cproject[m
[36m@@ -153,7 +153,7 @@[m
                             							[m
                             <option id="de.innot.avreclipse.toolchain.options.toolchain.size.app.release.708324178" name="Print Size" superClass="de.innot.avreclipse.toolchain.options.toolchain.size.app.release"/>[m
                             							[m
[31m-                            <option id="de.innot.avreclipse.toolchain.options.toolchain.avrdude.app.release.1086120664" name="AVRDude" superClass="de.innot.avreclipse.toolchain.options.toolchain.avrdude.app.release" value="true" valueType="boolean"/>[m
[32m+[m[32m                            <option id="de.innot.avreclipse.toolchain.options.toolchain.avrdude.app.release.1086120664" name="AVRDude" superClass="de.innot.avreclipse.toolchain.options.toolchain.avrdude.app.release" value="false" valueType="boolean"/>[m[41m[m
                             							[m
                             <targetPlatform id="de.innot.avreclipse.targetplatform.winavr.app.release.420385008" name="AVR Cross-Target" superClass="de.innot.avreclipse.targetplatform.winavr.app.release"/>[m
                             							[m
[36m@@ -193,7 +193,7 @@[m
                                 								[m
                                 <option id="de.innot.avreclipse.cpplinker.option.otherlinkargs.237428261" name="Other Arguments" superClass="de.innot.avreclipse.cpplinker.option.otherlinkargs" useByScannerDiscovery="false" value="" valueType="string"/>[m
                                 								[m
[31m-                                <option id="de.innot.avreclipse.cpplinker.option.libs.1235139071" name="Libraries (-l)" superClass="de.innot.avreclipse.cpplinker.option.libs" useByScannerDiscovery="false" valueType="libs"/>[m
[32m+[m[32m                                <option id="de.innot.avreclipse.cpplinker.option.libs.1235139071" name="Libraries (-l)" superClass="de.innot.avreclipse.cpplinker.option.libs" useByScannerDiscovery="false"/>[m[41m[m
                                 								[m
                                 <inputType id="de.innot.avreclipse.tool.cpplinker.input.803189994" name="OBJ Files" superClass="de.innot.avreclipse.tool.cpplinker.input">[m
                                     									[m
[36m@@ -215,7 +215,7 @@[m
                             							[m
                             <tool id="de.innot.avreclipse.tool.size.winavr.app.release.218458399" name="Print Size" superClass="de.innot.avreclipse.tool.size.winavr.app.release">[m
                                 								[m
[31m-                                <option id="de.innot.avreclipse.size.option.formatwithavr.668740957" name="Size Format" superClass="de.innot.avreclipse.size.option.formatwithavr" useByScannerDiscovery="false" value="de.innot.avreclipse.size.option.format.avr" valueType="enumerated"/>[m
[32m+[m[32m                                <option id="de.innot.avreclipse.size.option.formatwithavr.668740957" name="Size Format" superClass="de.innot.avreclipse.size.option.formatwithavr" useByScannerDiscovery="false" value="de.innot.avreclipse.size.option.format.berkeley" valueType="enumerated"/>[m[41m[m
                                 							[m
                             </tool>[m
                             							[m
[1mdiff --git a/Eclipse/m32_uart_buffer/uart_interpreter.cpp b/Eclipse/m32_uart_buffer/uart_interpreter.cpp[m
[1mindex 2039244..00261e0 100644[m
[1m--- a/Eclipse/m32_uart_buffer/uart_interpreter.cpp[m
[1m+++ b/Eclipse/m32_uart_buffer/uart_interpreter.cpp[m
[36m@@ -214,7 +214,7 @@[m [mstatic void cmd_process(void) {[m
 	cmd_parse();[m
 	for (uint8_t cmd = 0; cmd < NB_COMMANDS; cmd++) {[m
 [m
[31m-		if (!strcmp(cmd_table[cmd].str, (char *)cmd_buffer)) {[m
[32m+[m		[32mif (cmd_cmp(cmd_table[cmd].str, (char *)cmd_buffer)) {[m[41m[m
 			if(nbParams>0 && *params[0]=='?'){[m
 				uart_transmitnl(cmd_table[cmd].descr);[m
 				uart_prompt();[m
