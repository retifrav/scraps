# Find `php.ini` that is used by Apache

Use **vi** to create a `info.php` in your website directory:

`vi /var/www/info.php`

Press `i`, paste this:

`<?php phpinfo(); ?>`

Press `ESC` and then `SHIFT + Z + Z` to save and exit **vi**.

Open this in browser: http://YOUR-DOMAIN.com/info.php

Here's your `php.ini`:

![php.ini](img/php-ini.png?raw=true "php.ini")

Now you can delete `info.php`.