this.Dispatcher.Invoke(DispatcherPriority.Normal, new Action(() =>
    {
        txbx_log.Text += "ololo";
    }
    ));