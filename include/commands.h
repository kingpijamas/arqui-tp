#ifndef COMMANDS_H
#define COMMANDS_H

#include "../include/stdio.h"
#include "../include/stdlib.h"
#include "../include/string.h"

static const char logo[][80]={
	"       _                            ____    _____ ",
	"      | |                          / __ \\  / ____|",
	"  ___ | |__    __ _  _ __    __ _ | |  | || (___  ",
	" / __|| '_ \\  / _` || '_ \\  / _` || |  | | \\___ \\ ",
	"| (__ | | | || (_| || | | || (_| || |__| | ____) |",
	" \\___||_| |_| \\__,_||_| |_| \\__, | \\____/ |_____/ ",
	"                             __/ |                ",
	"                            |___/            ",
	""
};

void echo_cmd(int argc, char ** args);
void logo_cmd(int argc, char ** args);
void readc_test_cmd(int argc, char ** args);
void putChar_test_cmd(int argc, char ** args);
void printf_test_cmd(int argc, char ** args);
void getChar_test_cmd(int argc, char ** args);
void scanf_test_cmd(int argc, char ** args);
void help_cmd(int argc, char ** args);

void __print_help_line(const char * cmdName,const char * description);
#endif