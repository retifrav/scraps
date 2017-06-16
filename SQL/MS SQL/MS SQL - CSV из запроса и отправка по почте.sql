DECLARE @csv AS nvarchar(max)
DECLARE @cmd AS varchar(250)
DECLARE @filename AS nvarchar(max) = 'C:\temp\ololo.csv'

-- ну тут в курсоре писать очередную строку из полей и запятых
SET @csv = 'полеАдин,полеДва,полеТри'
-- а если найдёшь, как сделать перенос строки, то echo не каждый раз придётся вызывать, а один раз после курсора, и не >>, а >
SET @cmd = 'echo ' +  @csv + ' >> ' + @filename
EXECUTE master..xp_cmdshell @cmd

EXEC msdb.dbo.sp_send_dbmail
@profile_name = 'somemailrobot',
@recipients='ivanov@yandex.com',
@subject = 'CSV-файл',
@file_attachments=@filename,
@body = 'Ололо, текст письма',
@body_format = 'HTML';