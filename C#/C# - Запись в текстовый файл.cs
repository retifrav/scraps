using (StreamWriter writetext = File.AppendText("ololo.txt"))
{
    try { writetext.WriteLine("Ololo, world!"); } catch { }
}