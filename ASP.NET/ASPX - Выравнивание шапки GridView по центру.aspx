Кароч, суть в том, что шапка в конечном итоге превращается в теги <th>, потому надо просто задать новый CSS-класс для них:

<style type="text/css">
    .grd th
    {text-align:center;}
</style>

И потом в теге его указать <asp:GridView CssClass="grd">