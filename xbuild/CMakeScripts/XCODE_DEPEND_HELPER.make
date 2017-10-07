# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.kaleidoscope.Debug:
/Users/lanza/Documents/Kaleidoscope/xbuild/Debug/kaleidoscope:\
	/usr/local/lib/libLLVMSupport.a\
	/usr/local/lib/libLLVMCore.a\
	/usr/local/lib/libLLVMIRReader.a\
	/usr/local/lib/libLLVMAsmParser.a\
	/usr/local/lib/libLLVMBitReader.a\
	/usr/local/lib/libLLVMCore.a\
	/usr/local/lib/libLLVMBinaryFormat.a\
	/usr/local/lib/libLLVMSupport.a\
	/usr/local/lib/libLLVMDemangle.a
	/bin/rm -f /Users/lanza/Documents/Kaleidoscope/xbuild/Debug/kaleidoscope


PostBuild.kaleidoscope.Release:
/Users/lanza/Documents/Kaleidoscope/xbuild/Release/kaleidoscope:\
	/usr/local/lib/libLLVMSupport.a\
	/usr/local/lib/libLLVMCore.a\
	/usr/local/lib/libLLVMIRReader.a\
	/usr/local/lib/libLLVMAsmParser.a\
	/usr/local/lib/libLLVMBitReader.a\
	/usr/local/lib/libLLVMCore.a\
	/usr/local/lib/libLLVMBinaryFormat.a\
	/usr/local/lib/libLLVMSupport.a\
	/usr/local/lib/libLLVMDemangle.a
	/bin/rm -f /Users/lanza/Documents/Kaleidoscope/xbuild/Release/kaleidoscope


PostBuild.kaleidoscope.MinSizeRel:
/Users/lanza/Documents/Kaleidoscope/xbuild/MinSizeRel/kaleidoscope:\
	/usr/local/lib/libLLVMSupport.a\
	/usr/local/lib/libLLVMCore.a\
	/usr/local/lib/libLLVMIRReader.a\
	/usr/local/lib/libLLVMAsmParser.a\
	/usr/local/lib/libLLVMBitReader.a\
	/usr/local/lib/libLLVMCore.a\
	/usr/local/lib/libLLVMBinaryFormat.a\
	/usr/local/lib/libLLVMSupport.a\
	/usr/local/lib/libLLVMDemangle.a
	/bin/rm -f /Users/lanza/Documents/Kaleidoscope/xbuild/MinSizeRel/kaleidoscope


PostBuild.kaleidoscope.RelWithDebInfo:
/Users/lanza/Documents/Kaleidoscope/xbuild/RelWithDebInfo/kaleidoscope:\
	/usr/local/lib/libLLVMSupport.a\
	/usr/local/lib/libLLVMCore.a\
	/usr/local/lib/libLLVMIRReader.a\
	/usr/local/lib/libLLVMAsmParser.a\
	/usr/local/lib/libLLVMBitReader.a\
	/usr/local/lib/libLLVMCore.a\
	/usr/local/lib/libLLVMBinaryFormat.a\
	/usr/local/lib/libLLVMSupport.a\
	/usr/local/lib/libLLVMDemangle.a
	/bin/rm -f /Users/lanza/Documents/Kaleidoscope/xbuild/RelWithDebInfo/kaleidoscope




# For each target create a dummy ruleso the target does not have to exist
/usr/local/lib/libLLVMAsmParser.a:
/usr/local/lib/libLLVMBinaryFormat.a:
/usr/local/lib/libLLVMBitReader.a:
/usr/local/lib/libLLVMCore.a:
/usr/local/lib/libLLVMDemangle.a:
/usr/local/lib/libLLVMIRReader.a:
/usr/local/lib/libLLVMSupport.a:
