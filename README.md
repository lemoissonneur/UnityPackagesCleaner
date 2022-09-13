## UnityPackagesCleaner

remove unused packages from unity cache.

This tool works by scanning unity hub logs for unity projects path.
Then it compare the existing packages in unity's cache and the ones in your projects packages-lock.json file.





### details
- only projects opened with unity hub will be checked
- old projects may be ignored if they do are no longer in the logs
- both 'cache/packages' and 'cache/npm' are cleared
- made with Qt Creator  6.0.2
