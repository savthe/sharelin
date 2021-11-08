^[blue bold]Download details^[] (ID:^[bold white]$id^[])

Name:	^[bold white]$name^[]

Size:	^[bold white]$size^[] (^[bold white]$hrsize^[])

Done:	^[bold white]$progress %^[]

Src:	^[bold white]$downloaders^[]

Rate:	^[bold white]$rate ps^[]

SHA1:	^[bold white]$sha1^[]

TTR:	^[bold white]$ttr^[]

BP:	^[bold white]$bp^[]

ED2K:	^[bold white]$ed2k^[]

MD5:	^[bold white]$md5^[]

#if $tiger_min
TT:	^[bold white]available^[] (min: ^[bold white]$tiger_min^[])
#else
TT:	^[bold white]n/a^[]
#endif 

|
#foreach chunk_status chunk_size
#if $chunk_status = empty 
^[white]$chunk_size^[] 
#endif
#if $chunk_status = full
^[bold white]$chunk_size^[] 
#endif
#if $chunk_status = valid
^[bold blue]$chunk_size^[] 
#endif
#if $chunk_status = invalid
^[red]$chunk_size^[] 
#endif
#if $chunk_status = active
^[yellow]$chunk_size^[] 
#endif
#if $chunk_status = mixed
^[bold cyan]$chunk_size^[] 
#endif
#if $chunk_status = 0
^[bold magenta]$chunk_size^[] 
#endif
|
#next

#include downloaders.tpl
