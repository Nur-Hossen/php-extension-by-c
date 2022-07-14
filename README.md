**Installation Guidlines**
-----------------------------------------------------

**1. install php extensions:**<br>
php7.2-dev<br>
apache2<br>
php7.2-mysql<br>
libapache2-mod-php7.2<br>
libboost-date-time<br>
libboost-date-time-dev<br>
libboost-all-dev<br>

By command:<br>
(`sudo apt-get install build-essential php7.2 php7.2-dev php7.2-mysql libapache2-mod-php7.2 libboost-all-dev  libboost-date-time libboost-date-time-dev autoconf libtool bison re2c`)
<br>
Or<br>
you can install by synaptic package manager<br>
<br>
**2. Netbeans IDE installation process:**<br>
https://computingforgeeks.com/install-netbeans-ide-on-debian-ubuntu-and-linux-mint/ <br>
or<br>
goto ubuntu software center and search apache netbeans > then install<br>
<br>
**3. configuring Netbeasn:**<br>
 > Tools > Plugin > settings > check NetBeans Plugin Portal<br>
 > Tools > Plugin > Available Plugins > press Check for Newest button > search c++ > check this and install<br>
 > restart NetBeans<br>

**4. Open project by NetBeans and doing your business then clean and build.** <br>

**5. For Build by terminal command:**<br>
 > First we prepare the build environment for a PHP extension by:  `phpize` <br>
 > Then we configure the build and enable our extension by: `./configure --enable-php-helloworld` (php-helloworld    means      extension name) <br>
 > Finally, we can build it by: `make` <br>
  if make did not installed, install make by: `sudo make install` <br>
 > For clean make: `make -f Makefile clean`<br>

To learn more about Boost and zend
---------------------------------------
1. http://www.phpinternalsbook.com/index.html
2. https://stackoverflow.com/questions/3632160/how-to-make-a-php-extension/32575493
3. https://www.zend.com/sites/zend/files/pdfs/whitepaper-zend-php-extensions.pdf

To update schedule
---------------------------------------------
1. https://www.zend.com/php-references
2. https://wiki.php.net/phpng-upgrading



