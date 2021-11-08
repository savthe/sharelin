^[blue bold]Log^[] (Levels: basic, ^[bold white]advanced^[], ^[bold red]dev^[])
#foreach level line

#if $level = 0	
^[white]$line^[]
#endif
#if $level = 1	
^[bold white]$line^[]
#endif
#if $level = 2	
^[bold red]$line^[]
#endif
#next
