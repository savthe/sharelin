#foreach dlr_address dlr_status dlr_pushed dlr_rate dlr_qnum dlr_qsize dlr_left dlr_right dlr_length dlr_total

^[cyan]$dlr_address^[]
#if $dlr_pushed
^[bold white]p^[]
#endif
#if $dlr_status = idle
	^[bold white]thinking^[]
#endif
#if $dlr_status = connect
	^[bold white]connecting^[]
#endif
#if $dlr_status = reply
	^[bold white]talking^[]
#endif
#if $dlr_status = tiger
	^[bold white]fetching tt^[]
#endif
#if $dlr_status = file
	S:^[bold white]$dlr_left^[] - ^[bold white]$dlr_right^[] (^[bold white]$dlr_length^[]) 
#endif
#if $dlr_status = queued
	Q:^[bold white]$dlr_qnum^[] of ^[bold white]$dlr_qsize^[]
#endif
#if $dlr_rate != 0 B
 R:^[bold white]$dlr_rate ps^[]
#endif
#if $dlr_total != 0 B
 T:^[bold white]$dlr_total^[]
#endif
#next
