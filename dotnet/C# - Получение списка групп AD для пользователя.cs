using (PrincipalContext context = new PrincipalContext(ContextType.Domain))
{
    string loginAD = Context.User.Identity.Name;
    using (UserPrincipal user =
        UserPrincipal.FindByIdentity(context, loginAD))
    {
        if (user != null)
        {
            List<GroupPrincipal> result = new List<GroupPrincipal>();
            PrincipalSearchResult<Principal> groups = user.GetAuthorizationGroups();

            // iterate over all groups
            foreach (Principal p in groups)
            {
                // make sure to add only group principals
                if (p is GroupPrincipal)
                {
                    result.Add((GroupPrincipal)p);
                }
            }
        }
    }
}