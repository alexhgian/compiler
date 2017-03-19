Requirement:
Have this folder in your root project folder. p4exe should also be in the root.
Otherwise glc, gli, and p4exe cannot be accessed.

It should look something like this:
/ProjectRoot
   /p4exe
   /glc
   /gli
   /tests
      /testDirectory
      /test-all.sh
      /outfile.sh
      /test-all.py
      /test.sh

To run individual tests w/ bitcode for p4exe and glc as well as diffs:
./test.sh FOLDER_NAME TEST_NAME

For Example:
./test.sh arithmetic/ sub

Run all tests:
./test-all.sh
