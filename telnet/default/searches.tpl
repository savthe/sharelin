^[blue bold]Searches^[] (^[bold white]$total_searches^[])
#if $total_searches
#foreach id extensions criteria hits

$criteria  $extensions  (H:^[bold white]$hits^[] ID:^[bold white]$id^[])
#next
#endif
