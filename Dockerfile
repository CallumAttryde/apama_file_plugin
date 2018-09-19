FROM kpalf/apamacore:10.2.0.2_ubuntu_amd64 as builder
COPY . ${APAMA_WORK}/apama_file_transport
RUN apt-get update && apt-get install -y build-essential python
RUN cd ${APAMA_WORK}/apama_file_transport/plugin; make install
ENV \ 
	PYTHONPATH=${APAMA_HOME}/third_party/python/lib/python2.7/site-packages \
	LD_LIBRARY_PATH=${APAMA_WORK}/lib:${LD_LIBRARY_PATH}
RUN cd ${APAMA_WORK}/apama_file_transport/tests; pysys run | tee logfile && grep 'THERE WERE NO NON PASSES' logfile

FROM kpalf/apamacore:10.2.0.2_ubuntu_amd64
ENV \ 
	PYTHONPATH=${APAMA_HOME}/third_party/python/lib/python2.7/site-packages \
	LD_LIBRARY_PATH=${APAMA_WORK}/lib:${LD_LIBRARY_PATH}
COPY --from=builder ${APAMA_WORK}/lib ${APAMA_WORK}/lib

