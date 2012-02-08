
echo PAWPRINT = ${PAWPRINT:?"Need to set PAWPRINT; use 'export PAWPRINT=you'"}
echo tarball = ${1:?"Need to specify tarball; use 'submit source.tar.gz'"}
dir=/tmp/$PAWPRINT/4520

echo "\n\nextracting tarball..."
rm -rf $dir
mkdir -p $dir
tar -C $dir -xzf $1
cd $dir/*
rm -f *.o
make clean
cat /dev/null > $dir/4520out.log
echo "---------------------MAKE------------------" >> $dir/4520out.log
echo "\n\nbuilding source..."
make 2>&1 | tee -a $dir/4520out.log
echo "---------------------MAKE TEST------------------" >> $dir/4520out.log
echo "\n\nrunning tests..."
make test 2>&1 | tee -a $dir/4520out.log
echo "\n\nuploading source and output to server..."
echo "---------------------SOURCE------------------" >> $dir/4520out.log
for f in $(find .)
do
	file $f | grep text
	if [ "$?" = "0" ]
	then

		echo "\n-------------" >> $dir/4520out.log
		echo $f : >> $dir/4520out.log
		cat $f >> $dir/4520out.log
	fi
done
wget --keep-session-cookies --load-cookies $HOME/.4520cookies.txt --save-cookies $HOME/.4520cookies.txt --save-headers --post-file=$dir/4520out.log -O $dir/4520server.log http://babbage.cs.missouri.edu/\~rtdmr6/submit.php?pawprint=$PAWPRINT\&file=$1
if [ "$?" -ne "0" ]; then
	echo "You don't have wget on your system (maybe on a Mac?)"
	echo "Trying to use curl instead of wget..."
	curl --cookie $HOME/.4520cookies.txt --cookie-jar $HOME/.4520cookies.txt --data-binary @$dir/4520out.log --output $dir/4520server.log http://babbage.cs.missouri.edu/\~rtdmr6/submit.php?pawprint=$PAWPRINT\&file=$1
fi
cat $dir/4520out.log
echo "\n\nserver response:"
cat $dir/4520server.log
echo "\n\nDONE."
rm -rf $dir
