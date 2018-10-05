FROM kpalf/apamacore:10.2.0.2_ubuntu_amd64 as builder
COPY . ${APAMA_WORK}/apama_file_plugin
RUN apt-get update && apt-get install -y g++-4.8 make python
RUN ln -s /usr/bin/g++-4.8 /usr/bin/c++ && cd ${APAMA_WORK}/apama_file_plugin/plugin; make && make install
ENV \ 
	PYTHONPATH=${APAMA_HOME}/third_party/python/lib/python2.7/site-packages \
	LD_LIBRARY_PATH=${APAMA_WORK}/lib:${LD_LIBRARY_PATH}
RUN cd ${APAMA_WORK}/apama_file_plugin/tests; pysys run | tee logfile && grep 'THERE WERE NO NON PASSES' logfile

FROM kpalf/apamacore:10.2.0.2_ubuntu_amd64
ENV \ 
	PYTHONPATH=${APAMA_HOME}/third_party/python/lib/python2.7/site-packages \
	LD_LIBRARY_PATH=${APAMA_WORK}/lib:${LD_LIBRARY_PATH}
COPY --from=builder ${APAMA_WORK}/lib ${APAMA_WORK}/lib
COPY --from=builder ${APAMA_WORK}/monitors ${APAMA_WORK}/monitors

