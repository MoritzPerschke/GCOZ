#pragma once

typedef struct Message {
	int function_from_table;
	int num_called = 0;
}Message;