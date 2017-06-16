-- минимальная среди всех версий Windows максимальная длина командной строки
declare @cmd1 nvarchar(2047)

-- тут что-нибудь копируем
set @cmd1 = 'xcopy C:\1.txt C:\temp\1.txt /Y'
EXEC master..xp_cmdshell @cmd1

-- тут смотрим содержимое каталога
set @cmd1 = 'dir C:\temp\'
EXEC master..xp_cmdshell @cmd1