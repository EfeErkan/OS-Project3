compile: address_processing.h page_table.h frame_table.h circular_queue.h
	gcc pagism.c -o pagism
clean:
	rm out.txt pagism