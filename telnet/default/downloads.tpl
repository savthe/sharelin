^[blue bold]Downloads^[] (^[bold white]$total_downloads^[])
#if $total_downloads
#foreach download_id download_name download_size dl_status progress download_rate active_sources priority sources downloaders

#if $priority = -2
#def pri L
#endif 
#if $priority = -1
#def pri B
#endif 
#if $priority = 0
#def pri N
#endif 
#if $priority = 1
#def pri A
#endif 
#if $priority = 2
#def pri H
#endif 
#def stat A
#if $dl_status = queued
#def stat Q
#endif
#if $dl_status = paused
#def stat P
#endif
^[bold white]$pri $stat^[] ^[cyan]$download_name^[] ^[bold white]$download_size^[] R:^[bold white]$download_rate ps^[] P:^[bold white]$progress^[] S:^[bold white]$active_sources^[] ID:^[bold white]$download_id^[]
#next
#endif
