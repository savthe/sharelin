^[blue bold]Uploads^[] (^[bold white]$total_uploads^[])

^[bold blue]Active^[]
#foreach a_type a_name a_nick a_address a_rate a_size a_client

^[cyan]$a_name  (^[bold white]$a_nick^[]($a_client, $a_address) U:^[bold white]$a_size^[], R:^[bold white]$a_rate  KBps^[])
#next

^[bold blue]Queued^[]
#foreach q_type q_name q_nick q_address q_client

^[cyan]$q_name  (^[bold white]$q_nick^[]($q_client, $q_address)
#next

