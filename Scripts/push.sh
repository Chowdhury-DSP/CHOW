#!/bin/sh

copy_binary_files()
{
  cp -R $TRAVIS_BUILD_DIR/Builds/MacOSX/build/Release/CHOW.app $TRAVIS_BUILD_DIR/Bin/Mac/CHOW.app
  cp -R $TRAVIS_BUILD_DIR/Builds/MacOSX/build/Release/CHOW.component $TRAVIS_BUILD_DIR/Bin/Mac/CHOW.component
  cp -R $TRAVIS_BUILD_DIR/Builds/MacOSX/build/Release/CHOW.vst $TRAVIS_BUILD_DIR/Bin/Mac/CHOW.vst
  cp -R $TRAVIS_BUILD_DIR/Builds/MacOSX/build/Release/CHOW.vst3 $TRAVIS_BUILD_DIR/Bin/Mac/CHOW.vst3
}

setup_git()
{
  cd $TRAVIS_BUILD_DIR/
  git config --global user.email "travis@travis-ci.org"
  git config --global user.name "Travis CI"
}

commit_files()
{
  git checkout -b release
  git add Bin/Mac/
}

push_files()
{
  git remote add origin https://${GH_TOKEN}@github.com/jatinchowdhury18/CHOW.git > /dev/null 2>&1
  # git push --quiet --set-upstream origin-pages gh-pages
  git push -u origin release
}

copy_binary_files
setup_git
commit_files
push_files
