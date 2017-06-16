# How to update WordPress from the admin page

You need to allow one [special operation](http://www.hongkiat.com/blog/update-wordpress-without-ftp/) in your **WordPress** config file.

Open `/var/www/your-website/wp-config.php`, add this in the end of file:

```php
/** Sets up WordPress vars and included files. */
require_once(ABSPATH . 'wp-settings.php');

// for updates
define('FS_METHOD','direct');
```

Go to http://YOUR-DOMAIN/wp-admin/update-core.php. Perform updates simply by clicking `Update Now`.

Go back to `wp-config.php` and comment this string (it's not safe to leave it like this):

```php
//define('FS_METHOD','direct');
```