# CocoaPods

How does one make a CocoaPods package for a C++ library and how to publish/install it to/from JFrog Artifactory.

<!-- MarkdownTOC -->

- [Build server](#build-server)
- [Making a package](#making-a-package)
- [Installing a package from Artifactory](#installing-a-package-from-artifactory)

<!-- /MarkdownTOC -->

It is assumed that packing and publishing will happen on a Mac OS machine. It should have Ruby and CocoaPods gem [installed](https://guides.cocoapods.org/using/getting-started.html#sudo-less-installation).

## Build server

``` sh
$ nano ~/.bash_profile
```
``` sh
export GEM_HOME=$HOME/.gem
export PATH=$GEM_HOME/bin:$PATH
```
``` sh
$ source ~/.bash_profile
```

``` sh
$ gem install cocoapods --user-install
$ ~/.gem/ruby/3.1.0/bin/pod --version
$ ~/.gem/ruby/3.1.0/bin/pod --help
```

## Making a package

This is not a "proper" package with Xcode integration, as it just packs everything as resources.

``` sh
$ mkdir SomeLibrary && cd $_
$ ~/.gem/ruby/3.1.0/bin/pod spec create SomeLibrary
```

Edit `SomeLibrary.podspec`:

``` ruby
Pod::Spec.new do |spec|

  # ―――  Spec Metadata  ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #

  spec.name = "$PACKAGE_NAME"
  spec.version = "$PACKAGE_VERSION"
  spec.summary = "$PACKAGE_SUMMARY"

  spec.description = <<-DESC
      $PACKAGE_DESCRIPTION
    DESC

  spec.homepage = "$PACKAGE_HOMEPAGE"


  # ―――  Spec License  ――――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #

  spec.license = {
      :type => "Proprietary", # "Custom"
      :file => "license.pdf"
  }


  # ――― Author Metadata  ――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #

  spec.author = "Some Company"


  # ――― Platform Specifics ――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #

  spec.platform = :ios
  # spec.platform     = :ios, "15.0"

  #  When using multiple platforms
  # spec.ios.deployment_target = "5.0"
  # spec.osx.deployment_target = "10.7"
  # spec.watchos.deployment_target = "2.0"
  # spec.tvos.deployment_target = "9.0"


  # ――― Source Location ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  #  Specify the location from where the source should be retrieved.
  #  Supports git, hg, bzr, svn and HTTP.
  #

  # http://markspanbroek.github.io/2016/04/14/binary-cocoapods.html
  #
  # don't know how does this suppose to work. Right now it just creates empty folders on `pod install`
  # and fetches .podspec file alone, nothing else. Will keep it here just in case for future,
  # when we know more
  #
  #spec.default_subspec = "Binary"
  #
  #spec.subspec "Source" do |source|
  #  source.source = {
  #    :git => "$SOURCES_REPOSITORY_URL",
  #    :tag => "#{spec.version}"
  #  }
  #  #source.source_files = "**/*"
  #end
  #
  #spec.subspec "Binary" do |binary|
  #  binary.source = {
  #    :http => "$PACKAGE_DOWNLOAD_URL"
  #  }
  #end

  spec.resources = "**/*"


  # ――― Source Code ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  # spec.source_files  = "Classes", "Classes/**/*.{h,m}"
  #  Not including the public_header_files will make all headers public.
  # spec.public_header_files = "Classes/**/*.h"

  # spec.exclude_files = "Classes/Exclude"


  # ――― Project Settings ――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #
  #
  #  If your library depends on compiler flags you can set them in the xcconfig hash
  #  where they will only apply to your library. If you depend on other Podspecs
  #  you can include multiple dependencies to ensure it works.

  # spec.requires_arc = true

  # spec.xcconfig = { "HEADER_SEARCH_PATHS" => "$(SDKROOT)/usr/include/libxml2" }
  # spec.dependency "JSONKit", "~> 1.4"
end
```

Having environment variables (`$`) here, or rather in a separate `package-template.podspec`, you can reuse the same specification for making different packages:

``` sh
$ export PACKAGE_REVISION="12345"
$ export PACKAGE_NAME="SomeLibrary"
$ export PACKAGE_VERSION="0.0.$PACKAGE_REVISION"
$ export PACKAGE_SUMMARY="Some summary"
$ export PACKAGE_DESCRIPTION="Some description."
$ export PACKAGE_HOMEPAGE="https://some.host/SomeLibrary"
$ export SOURCES_REPOSITORY_URL="https://git.some.host/SomeLibrary.git"
$ export PACKAGE_DOWNLOAD_URL="https://some.host/files/master/12345/SomeLibrary.zip"

$ envsubst < ./package-template.podspec > ./SomeLibrary.podspec
```

Check the final specification:

``` sh
$ ~/.gem/ruby/3.1.0/bin/pod spec lint ./SomeLibrary.podspec
```

Add `cmake`/`include`/`lib` there and pack everything:

``` sh
$ ls -L1
cmake/
include/
lib/
SomeLibrary.podspec

$ brew install gnu-tar
$ gtar czvf package-0.0.12345.tar.gz ./*
$ curl -H "X-JFrog-Art-Api:BUILDBOT-API-KEY-GOES-HERE" -X PUT https://artifactory.some.host/artifactory/some-company-cocoapods/SomeLibrary/ -T ./package-0.0.12345.tar.gz --fail --silent --show-error
```

## Installing a package from Artifactory

``` sh
$ nano ~/.netrc
```
```
machine artifactory.some.host
login YOUR-EMAIL
password YOUR-TOKEN
```

Create an Xcode project, as it cannot install without it, but since you are using CocoaPods, then most likely you do already an Xcode project. Inside project folder (*on the same level as `*.xcodeproj`*) create a `Podfile`:

``` ruby
plugin "cocoapods-art", :sources => [
  "some-company-cocoapods"
]

use_frameworks!

target "SAME-NAME-AS-IN-XCODE-PROJECT" do
  pod "SomeLibrary"
end
```

Then:

``` sh
$ gem install cocoapods-art
$ ~/.gem/ruby/3.1.0/bin/pod repo-art add some-company-cocoapods "https://artifactory.some.host/artifactory/api/pods/some-company-cocoapods"
$ ~/.gem/ruby/3.1.0/bin/pod repo-art update some-company-cocoapods

$ cd /path/to/your/project
$ ~/.gem/ruby/3.1.0/bin/pod install
$ ls -L1 ./Pods/SomeLibrary/
cmake/
include/
lib/
SomeLibrary.podspec
```

It also keeps stuff in `/Users/YOURNAME/Library/Caches/CocoaPods/Pods/Release`, so it doesn't always download stuff from server.

The `pod repo-art update some-company-cocoapods` apparently needs to be run every time to fetch possible updates from Artifactory. The thing seems to update a local Git repository.
