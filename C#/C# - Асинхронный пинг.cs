Есть форма, на ней кнопка button1 и TextBox с именем label1.

// обработчик нажатия кнопки
private void button1_Click(object sender, EventArgs e)
{
    new Thread(pingSome).Start();
}

// операции треда
public void pingSome()
{
    string who = "ya.ru";
    AutoResetEvent waiter = new AutoResetEvent(false);

    Ping pingSender = new Ping();
    pingSender.PingCompleted += new PingCompletedEventHandler(PingCompletedCallback);

    // сколько будем пересылать в одном пинге
    byte[] buffer = new byte[32];

    // таймаут 6 секунд
    int timeout = 6000;

    // 64 хопа, нельзя фрагментировать
    PingOptions options = new PingOptions(64, true);
    for (int i = 0; i < 10; i++)
    {
        pingSender.SendAsync(who, timeout, buffer, options, waiter);
        Thread.Sleep(1000);
        waiter.WaitOne();
    }
}

// колбек треда
public void PingCompletedCallback(object sender, PingCompletedEventArgs e)
{
    // если отменилась операция, сказать об этом
    if (e.Cancelled)
    {
        MessageBox.Show(
            "Пинг отменился.",
            "Отменено",
            MessageBoxButtons.OK,
            MessageBoxIcon.Warning
            );

        // продолжить выполнение главного треда
        ((AutoResetEvent)e.UserState).Set();
    }

    // если ошибка, показать
    if (e.Error != null)
    {
        MessageBox.Show(
            "При пинге произошла ошибка: " + e.Error.ToString() + ".",
            "Ошибка",
            MessageBoxButtons.OK,
            MessageBoxIcon.Error
            );

        // продолжить выполнение главного треда
        ((AutoResetEvent)e.UserState).Set();
    }

    PingReply reply = e.Reply;

    if (reply.Status == IPStatus.Success)
    {
        this.Invoke((MethodInvoker)delegate()
            {
                label1.Text += "Пингуется. Время: "
                    + reply.RoundtripTime
                    + Environment.NewLine;
            });
    }
    else
    {
        this.Invoke((MethodInvoker)delegate()
            {
                label1.Text += "Не пингуется." + Environment.NewLine;
            });
    }

    // продолжить выполнение главного треда
    ((AutoResetEvent)e.UserState).Set();
}