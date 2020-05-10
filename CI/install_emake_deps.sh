#!/bin/bash

set -e

if [ "$TRAVIS_OS_NAME" != "osx" ]; then
  # new protobuf
  sudo add-apt-repository -y ppa:maarten-fonville/protobuf;

  # new gcc 
  sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
  
  # new clang
  wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | sudo apt-key add -
  sudo apt-add-repository "deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-10 main"

  # new boost for old travis
  if [ "$COMPILER" == "Android" ]; then
    sudo add-apt-repository -y ppa:mhier/libboost-latest;
  else
    # new lcov
    sudo add-apt-repository -y ppa:cheeseboy16/travis-backports
  fi

  sudo apt-get update --option Acquire::Retries=100 --option Acquire::http::Timeout="60";
  sudo apt-get -y install gcc-9 g++-9 cpp-9 build-essential libprotobuf-dev protobuf-compiler zlib1g-dev libglm-dev libpng-dev \
    clang-10 lldb-10 lld-10

  sudo update-alternatives --remove-all cpp;
  sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 \
              15 \
              --slave   /usr/bin/g++ g++ /usr/bin/g++-9 \
              --slave   /usr/bin/gcov gcov /usr/bin/gcov-9 \
              --slave   /usr/bin/gcov-dump gcov-dump /usr/bin/gcov-dump-9 \
              --slave   /usr/bin/gcov-tool gcov-tool /usr/bin/gcov-tool-9 \
              --slave   /usr/bin/gcc-ar gcc-ar /usr/bin/gcc-ar-9 \
              --slave   /usr/bin/gcc-nm gcc-nm /usr/bin/gcc-nm-9 \
              --slave   /usr/bin/gcc-ranlib gcc-ranlib /usr/bin/gcc-ranlib-9;

  g++ --version

  # Remove all existing alternatives
  sudo update-alternatives --remove-all llvm
  sudo update-alternatives --remove-all clang

  # llvm
  sudo update-alternatives \
  --install /usr/lib/llvm              		llvm             		    /usr/lib/llvm-10 20 \
  --slave   /usr/bin/llvm-ar           		llvm-ar          		    /usr/bin/llvm-ar-10 \
  --slave   /usr/bin/llvm-as           		llvm-as          		    /usr/bin/llvm-as-10 \
  --slave   /usr/bin/llvm-bcanalyzer   		llvm-bcanalyzer  		    /usr/bin/llvm-bcanalyzer-10 \
  --slave   /usr/bin/llvm-cat       		    llvm-cat      			    /usr/bin/llvm-cat-10  \
  --slave   /usr/bin/llvm-cfi-verify      	llvm-cfi-verify      	    /usr/bin/llvm-cfi-verify-10  \
  --slave   /usr/bin/llvm-config       		llvm-config      		    /usr/bin/llvm-config-10  \
  --slave   /usr/bin/llvm-cov          		llvm-cov         		    /usr/bin/llvm-cov-10 \
  --slave   /usr/bin/llvm-c-test          	llvm-c-test         	    /usr/bin/llvm-c-test-10 \
  --slave   /usr/bin/llvm-cvtres          	llvm-cvtres         	    /usr/bin/llvm-cvtres-10 \
  --slave   /usr/bin/llvm-cxxdump          	llvm-cxxdump         	    /usr/bin/llvm-cxxdump-10 \
  --slave   /usr/bin/llvm-cxxfilt         	llvm-cxxfilt         	    /usr/bin/llvm-cxxfilt-10 \
  --slave   /usr/bin/llvm-cxxmap          	llvm-cxxmap        		    /usr/bin/llvm-cxxmap-10 \
  --slave   /usr/bin/llvm-diff         		llvm-diff       		    /usr/bin/llvm-diff-10 \
  --slave   /usr/bin/llvm-dis          		llvm-dis         		    /usr/bin/llvm-dis-10 \
  --slave   /usr/bin/llvm-dlltool         	llvm-dlltool         	    /usr/bin/llvm-dlltool-10 \
  --slave   /usr/bin/llvm-dwarfdump    		llvm-dwarfdump   		    /usr/bin/llvm-dwarfdump-10 \
  --slave   /usr/bin/llvm-dwp          		llvm-dwp         		    /usr/bin/llvm-dwp-10 \
  --slave   /usr/bin/llvm-elfabi          	llvm-elfabi         	    /usr/bin/llvm-elfabi-10 \
  --slave   /usr/bin/llvm-exegesis          	llvm-exegesis         	    /usr/bin/llvm-exegesis-10 \
  --slave   /usr/bin/llvm-extract          	llvm-extract         	    /usr/bin/llvm-extract-10 \
  --slave   /usr/bin/llvm-lib          		llvm-lib         		    /usr/bin/llvm-lib-10 \
  --slave   /usr/bin/llvm-link         		llvm-link        		    /usr/bin/llvm-link-10 \
  --slave   /usr/bin/llvm-lto2           		llvm-lto2          		    /usr/bin/llvm-lto2-10  \
  --slave   /usr/bin/llvm-lto           		llvm-lto          		    /usr/bin/llvm-lto-10  \
  --slave   /usr/bin/llvm-mc           		llvm-mc          		    /usr/bin/llvm-mc-10 \
  --slave   /usr/bin/llvm-mca           		llvm-mca          		    /usr/bin/llvm-mca-10 \
  --slave   /usr/bin/llvm-modextract          llvm-modextract             /usr/bin/llvm-modextract-10 \
  --slave   /usr/bin/llvm-mt           		llvm-mt          		    /usr/bin/llvm-mt-10 \
  --slave   /usr/bin/llvm-nm           		llvm-nm          		    /usr/bin/llvm-nm-10 \
  --slave   /usr/bin/llvm-objcopy      		llvm-objcopy     		    /usr/bin/llvm-objcopy-10 \
  --slave   /usr/bin/llvm-objdump      		llvm-objdump     		    /usr/bin/llvm-objdump-10 \
  --slave   /usr/bin/llvm-opt-report          llvm-opt-report             /usr/bin/llvm-opt-report-10 \
  --slave   /usr/bin/llvm-pdbutil          	llvm-pdbutil          	    /usr/bin/llvm-pdbutil-10  \
  --slave   /usr/bin/llvm-PerfectShuffle      llvm-PerfectShuffle         /usr/bin/llvm-PerfectShuffle-10 \
  --slave   /usr/bin/llvm-profdata          	llvm-profdata         	    /usr/bin/llvm-profdata-10 \
  --slave   /usr/bin/llvm-ranlib      		llvm-ranlib      		    /usr/bin/llvm-ranlib-10 \
  --slave   /usr/bin/llvm-rc          		llvm-rc         		    /usr/bin/llvm-rc-10 \
  --slave   /usr/bin/llvm-readelf          	llvm-readelf        	    /usr/bin/llvm-readelf-10 \
  --slave   /usr/bin/llvm-readobj      		llvm-readobj     		    /usr/bin/llvm-readobj-10 \
  --slave   /usr/bin/llvm-rtdyld       		llvm-rtdyld      		    /usr/bin/llvm-rtdyld-10 \
  --slave   /usr/bin/llvm-size         		llvm-size        		    /usr/bin/llvm-size-10 \
  --slave   /usr/bin/llvm-split         		llvm-split        		    /usr/bin/llvm-split-10 \
  --slave   /usr/bin/llvm-stress       		llvm-stress      		    /usr/bin/llvm-stress-10 \
  --slave   /usr/bin/llvm-strings         	llvm-strings        	    /usr/bin/llvm-strings-10 \
  --slave   /usr/bin/llvm-strip         		llvm-strip        		    /usr/bin/llvm-strip-10 \
  --slave   /usr/bin/llvm-symbolizer   		llvm-symbolizer  		    /usr/bin/llvm-symbolizer-10 \
  --slave   /usr/bin/llvm-tblgen       		llvm-tblgen      		    /usr/bin/llvm-tblgen-10 \
  --slave   /usr/bin/llvm-undname       		llvm-undname      		    /usr/bin/llvm-undname-10 \
  --slave   /usr/bin/llvm-xray       		    llvm-xray      			    /usr/bin/llvm-xray-10

  # clang
  sudo update-alternatives \
  --install /usr/bin/clang                	clang        			    /usr/bin/clang-10 10 \
  --slave   /usr/bin/clang++              	clang++      			    /usr/bin/clang++-10 \
  --slave   /usr/bin/clang-apply-replacements	clang-apply-replacements	/usr/bin/clang-apply-replacements-10 \
  --slave   /usr/bin/clang-change-namespace	clang-change-namespace		/usr/bin/clang-change-namespace-10 \
  --slave   /usr/bin/clang-check			    clang-check			        /usr/bin/clang-check-10 \
  --slave   /usr/bin/clang-cl			        clang-cl			        /usr/bin/clang-cl-10 \
  --slave   /usr/bin/clang-cpp			    clang-cpp			        /usr/bin/clang-cpp-10 \
  --slave   /usr/bin/clangd			        clangd				        /usr/bin/clangd-10 \
  --slave   /usr/bin/clang-extdef-mapping		clang-extdef-mapping		/usr/bin/clang-extdef-mapping-10 \
  --slave   /usr/bin/clang-format			    clang-format			    /usr/bin/clang-format-10 \
  --slave   /usr/bin/clang-format-diff		clang-format-diff		    /usr/bin/clang-format-diff-10 \
  --slave   /usr/bin/clang-import-test		clang-import-test		    /usr/bin/clang-import-test-10 \
  --slave   /usr/bin/clang-include-fixer		clang-include-fixer		    /usr/bin/clang-include-fixer-10 \
  --slave   /usr/bin/clang-offload-bundler	clang-offload-bundler		/usr/bin/clang-offload-bundler-10 \
  --slave   /usr/bin/clang-query			    clang-query			        /usr/bin/clang-query-10 \
  --slave   /usr/bin/clang-refactor		    clang-refactor			    /usr/bin/clang-refactor-10 \
  --slave   /usr/bin/clang-rename			    clang-rename			    /usr/bin/clang-rename-10 \
  --slave   /usr/bin/clang-reorder-fields		clang-reorder-fields		/usr/bin/clang-reorder-fields-10 \
  --slave   /usr/bin/scan-view             	scan-view              		/usr/bin/scan-view-10 \
  --slave   /usr/bin/scan-build            	scan-build             		/usr/bin/scan-build-10 \
  --slave   /usr/bin/scan-build-py         	scan-build-py          		/usr/bin/scan-build-py-10 \
  --slave   /usr/bin/bugpoint            		bugpoint             		/usr/bin/bugpoint-10 \
  --slave   /usr/bin/c-index-test          	c-index-test           		/usr/bin/c-index-test-10 \
  --slave   /usr/bin/diagtool            		diagtool             		/usr/bin/diagtool-10 \
  --slave   /usr/bin/find-all-symbols      	find-all-symbols       		/usr/bin/find-all-symbols-10 \
  --slave   /usr/bin/git-clang-format		    git-clang-format		    /usr/bin/git-clang-format-10 \
  --slave   /usr/bin/hmaptool            		hmaptool             		/usr/bin/hmaptool-10 \
  --slave   /usr/bin/modularize            	modularize             		/usr/bin/modularize-10 \
  --slave   /usr/bin/obj2yaml            		obj2yaml             		/usr/bin/obj2yaml-10 \
  --slave   /usr/bin/opt                  	opt                   		/usr/bin/opt-10 \
  --slave   /usr/bin/sancov                	sancov                 		/usr/bin/sancov-10 \
  --slave   /usr/bin/sanstats            		sanstats             		/usr/bin/sanstats-10 \
  --slave   /usr/bin/verify-uselistorder      verify-uselistorder         /usr/bin/verify-uselistorder-10 \
  --slave   /usr/bin/wasm-ld            		wasm-ld             		/usr/bin/wasm-ld-10 \
  --slave   /usr/bin/yaml2obj            		yaml2obj             		/usr/bin/yaml2obj-10 \
  --slave   /usr/bin/yaml-bench            	yaml-bench             		/usr/bin/yaml-bench-10 \
  --slave   /usr/bin/lld                  	lld          			    /usr/bin/lld-10 \
  --slave   /usr/bin/lld-link           		lld-link              		/usr/bin/lld-link-10 \
  --slave   /usr/bin/lli-child-target         lli-child-target          	/usr/bin/lli-child-target-10 \
  --slave   /usr/bin/lli                  	lli          			    /usr/bin/lli-10 \
  --slave   /usr/bin/lldb                  	lldb                   		/usr/bin/lldb-10 \
  --slave   /usr/bin/lldb-argdumper           lldb-argdumper              /usr/bin/lldb-argdumper-10 \
  --slave   /usr/bin/lldb-mi               	lldb-mi                  	/usr/bin/lldb-mi-10 \
  --slave   /usr/bin/lldb-server           	lldb-server            		/usr/bin/lldb-server-10 \
  --slave   /usr/bin/lldb-test           		lldb-test            		/usr/bin/lldb-test-10 \
  --slave   /usr/bin/lldb-vscode           	lldb-vscode            		/usr/bin/lldb-vscode-10 \

  # make system default
  sudo update-alternatives --install /usr/bin/cc  cc  /usr/bin/clang      100
  sudo update-alternatives --install /usr/bin/c++ c++ /usr/bin/clang++    100

  clang++ --version
fi

if [ "$COMPILER" == "Android" ]; then
  sudo apt-get -y install libboost1.67-dev
elif [ "$TRAVIS_OS_NAME" == "linux" ]; then
  sudo apt-get -y install libboost-program-options-dev pulseaudio libpugixml-dev libyaml-cpp-dev rapidjson-dev
elif [ "$TRAVIS_OS_NAME" == "osx" ]; then
  brew upgrade gcc || brew install gcc || brew link --overwrite gcc;
  brew install protobuf pugixml yaml-cpp rapidjson
fi
