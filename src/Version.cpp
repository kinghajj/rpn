/* This version of the getVersion() function is used for platforms that
 * can't get extra version information from a Git repository.
 */

#ifdef RPN_GITLESS
namespace RPN
{
	const char *getVersion()
	{
		return "gitless";
	}
}
#endif
