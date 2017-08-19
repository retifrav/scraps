// тут ещё масса всякого: http://www.codeproject.com/Articles/18102/Howto-Almost-Everything-In-Active-Directory-via-C

// правда, получает не все группы, да и получаемые какие-то не олло

List<string> groups = new List<string>();
foreach (System.Security.Principal.IdentityReference group in
    System.Web.HttpContext.Current.Request.LogonUserIdentity.Groups)
{
    groups.Add(group.Translate(typeof
        (System.Security.Principal.NTAccount)).ToString());
}