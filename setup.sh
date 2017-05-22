#!/bin/bash -e

case `uname` in
	Linux) ECHO="echo -e" ;;
	*) ECHO="echo" ;;
esac

usage() {
	$ECHO "setup.sh [options]"
	$ECHO
	$ECHO "Options:"
	$ECHO "-p            \tinstall Pythia 8.226"
	$ECHO "-e            \tlink Pythia 8.226 with HepMC"
	$ECHO "-d            \tinstall Delphes"
	$ECHO "-h            \tdisplay this message and exit"
	exit $1
}

CUR_DIR=`pwd`
INSTALL_PYTHIA=""
INSTALL_HEPMC=""
INSTALL_DELPHES=""
# check arguments
while getopts "ped" opt; do
	case "$opt" in
	p) INSTALL_PYTHIA=yes
	;;
	e) INSTALL_HEPMC=yes
	;;
	d) INSTALL_DELPHES=yes
	;;
	h) usage 0
	;;
	esac
done

if [ -z "$INSTALL_PYTHIA" ] && [ -z "$INSTALL_DELPHES" ]; then
	usage 1
fi

# setup compiler
source /cvmfs/sft.cern.ch/lcg/views/LCG_87/x86_64-slc6-gcc49-opt/setup.sh

if [ -n "$INSTALL_PYTHIA" ]; then
	cd $CUR_DIR
	# get pythia8 source and compile
	wget -q http://home.thep.lu.se/~torbjorn/pythia8/pythia8226.tgz
	tar -xzf pythia8226.tgz
	export PYTHIA8=$CUR_DIR/pythia8226
	export PYTHIA8DATA=$PYTHIA8/share/Pythia8/xmldoc
	cd pythia8226/

	# link to HepMC
	if [ -n "$INSTALL_HEPMC" ]; then
		ln -s /cvmfs/cms.cern.ch/slc6_amd64_gcc491/external/hepmc/2.06.07-cms/lib/libHepMC.a libHepMC.a
		mkdir HepMC
		cd HepMC
		ln -s /cvmfs/cms.cern.ch/slc6_amd64_gcc491/external/hepmc/2.06.07-cms/include/HepMC/* .
		cd ..
		./configure --with-hepmc2
	fi

	# build
	gmake

	# cleanup
	cd $CUR_DIR
	rm pythia8226.tgz
fi

if [ -n "$INSTALL_DELPHES" ]; then
	cd $CUR_DIR
	# get delphes source and compile
	wget -q http://cp3.irmp.ucl.ac.be/downloads/Delphes-3.4.1.tar.gz
	tar -zxf Delphes-3.4.1.tar.gz
	cd Delphes-3.4.1
	make
	# link to card
	ln -s $CUR_DIR/darkgen2/delphes_card_CMS_imp.tcl delphes_card_CMS_imp.tcl

	# cleanup
	cd $CUR_DIR
	rm Delphes-3.4.1.tar.gz
fi
