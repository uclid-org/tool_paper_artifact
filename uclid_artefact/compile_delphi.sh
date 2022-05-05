CURRENTLOC=$(pwd)
echo $CURRENTLOC
echo "recompiling CBMC"
cd $CURRENTLOC/delphi/lib/cbmc/src; make clean; make -j8
echo "recompiling delphi"
cd $CURRENTLOC/delphi/src; make clean; make -j8;
cd $CURRENTLOC
echo "copy across binary"
cp $CURRENTLOC/delphi/src/delphi/delphi /uclid_artefact/bin/delphi
