void	SplitCommand() {
	int i, p;
	n_parms = 0;
	for (i = 0; i < MAX_PARMS; i++)parms[i] = NULL;

	for (i = 0, p = 0; cmd_buffer[i] != NULLCHAR; i++) {
		if (cmd_buffer[i] == SPACE) {
			cmd_buffer[i] = NULLCHAR;
			parms[p++] = &cmd_buffer[i] + 1;
			n_parms++;
		}
	}
}
void	ProcessCommand () {
	int cmd;
	if (cmd_buffer[0] == NULLCHAR) {
		Serial.print("QM> ");
		return;
	}
	SplitCommand(); 
	for (cmd = 0; cmd < N_COMMANDS; cmd++) {
		if (strcmp (cmd_table[cmd].cmd, (char *)cmd_buffer) == 0) {
			cmd_table[cmd].func();
			goto done;
		}
	}
	Serial << "wtf?" << endl;

	done:
	cmd_buffer_index = 0;
	cmd_buffer[0] = NULLCHAR;
	Serial << "QM> ";
}

void	command_handling_loop() {
	char c;
	
	if (Serial.peek() > 0) {
		c = Serial.read();
		if (ESC == c) {
			while (Serial.available() < 2) {};
			c = Serial.read();
			c = Serial.read();	
			switch (c) {
				case 'A':  // up arrow
					// copy the last command into the command buffer
					// then echo it to the terminal and set the 
					// the buffer's index pointer to the end 
					memcpy(cmd_buffer, last_cmd, sizeof(last_cmd));
					cmd_buffer_index = strlen (cmd_buffer);
					Serial << cmd_buffer;
					break;
			}
		} else {
			c = tolower(c);
			switch (c) {
				
				case TAB:  // TAB runs the last command
					memcpy(cmd_buffer, last_cmd, sizeof(cmd_buffer));
					ProcessCommand ();
					break;

				case BACKSPACE:
					if (cmd_buffer_index > 0) {
						cmd_buffer[--cmd_buffer_index] = NULLCHAR;
						Serial << _BYTE(BACKSPACE) << SPACE << _BYTE(BACKSPACE);
					}
					break;

				case LF:
					ProcessCommand ();
					Serial.flush();		// remove any following CR
					break;
				
				case CR:
					ProcessCommand ();
					Serial.flush();		// remove any following LF
					break;

				default:  // just put the char in the buffer
					cmd_buffer[cmd_buffer_index++] = c;
					cmd_buffer[cmd_buffer_index] = NULLCHAR;
					Serial.print (c);
			}
		}
	}
}
