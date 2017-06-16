http://hpop.sourceforge.net/exampleFetchAllMessages.php
http://stackoverflow.com/questions/10317411/how-to-save-email-attachment-using-openpop/21855722#21855722

using OpenPop.Pop3;

using (Pop3Client client = new Pop3Client())
{
    //if (client.Connected) { client.Disconnect(); }
    client.Connect("pop.yandex.ru", 995, true);
    client.Authenticate(@"mahlogin", "mahpassword");
    
    try
    {
        int messageCount = client.GetMessageCount();
        if (messageCount == 0)
        {
            txtbx.Text = "Новых писем на сервере нет.";
            return;
        }

        List<OpenPop.Mime.Message> allMessages = new List<OpenPop.Mime.Message>(messageCount);

        for (int i = 1; i <= messageCount; i++)
        {
            allMessages.Add(client.GetMessage(i));
        }

        foreach (OpenPop.Mime.Message mail in allMessages)
        {
            txtbx.Text += mail.Headers.From.ToString()
                + " - " + mail.Headers.Subject
                + Environment.NewLine;

            foreach (var attachment in mail.FindAllAttachments())
            {
                if (!Directory.Exists("attachs")) { Directory.CreateDirectory("attachs"); }
                string filePath = Path.Combine("attachs", attachment.FileName);
                //if (attachment.FileName.Equals("blabla.pdf"))
                //{
                    using (FileStream Stream = new FileStream(filePath, FileMode.Create))
                    using (BinaryWriter BinaryStream = new BinaryWriter(Stream))
                    {
                        BinaryStream.Write(attachment.Body);
                    }
                //}

            }

        }
    }
    catch (Exception ex)
    {
        txtbx.Text = ex.Message;
    }
}