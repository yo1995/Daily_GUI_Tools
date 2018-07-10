char buffer[16];
_itoa_s(score_type, buffer, 10);
MessageBox(0, buffer, "Window mismatch!", MB_ICONERROR);