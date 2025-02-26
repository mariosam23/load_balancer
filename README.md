# Load Balancer

## Description:

### `INIT_LOAD_BALANCER` Command

This is the initial command, in which memory is allocated for the load balancer and the fields of its structure are initialized.

### `LOADER_ADD_SERVER` Command

Command that adds servers to the imaginary circle. Before adding, I check if reallocation is needed to expand the circle. Then, as I add servers, I also check if rebalancing is necessary, which means if the size of the source server's dictionary is not zero. I find the position where objects should be rebalanced, followed by the actual movement of objects.

### `LOADER_REMOVE_SERVER` Command

I determine the position of the server to be removed using binary search. Then, similar to the add function, I determine the position of the server where objects should be moved and perform this action. Finally, I move servers to the last positions and free their memory.

### `LOADER_STORE` Command

I find the position where a new object should be added, then perform the actual addition through the "server_store" function, which puts the key-value pair in the dictionary of the appropriate server.

### `LOADER_RETRIEVE` Command

I find the position of the server that contains the hash of the key received as a parameter using binary search. Then, I call the "server_retrieve" function, which extracts the value from the server's dictionary.

### `FREE_LOAD_BALANCER` Command

Performs deallocation of all resources and allows the program to terminate optimally.
