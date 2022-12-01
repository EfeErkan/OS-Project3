compile: address_processing.h page_table.h frame_table.h circular_queue.h
	gcc pagesim.c -o pagesim
clean:
	rm out.txt pagesim