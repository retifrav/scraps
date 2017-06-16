-- запускается, понятное дело, из консоли на сервере, где есть MS SQL
sqlcmd -S SERVERNAME -d DATABASENAME -U USERNAME -P PASSWORD -Q "SELECT * FROM sometable" -s "," -o "C:\temp\ololo.csv"