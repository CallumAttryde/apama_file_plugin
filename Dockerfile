FROM kpalf/apamacore:10.3.0.1_ubuntu_amd64 as builder
COPY . ${APAMA_WORK}/apama_file_plugin
USER root
RUN apt-get update && apt-get install -y g++ make python3
RUN cd ${APAMA_WORK}/apama_file_plugin/plugin; make && make install
RUN cd ${APAMA_WORK}/apama_file_plugin/tests; pysys run | tee logfile && grep 'THERE WERE NO NON PASSES' logfile

FROM kpalf/apamacore:10.3.0.1_ubuntu_amd64
COPY --from=builder ${APAMA_WORK}/lib ${APAMA_WORK}/lib
COPY --from=builder ${APAMA_WORK}/monitors ${APAMA_WORK}/monitors
USER sagadmin:sagadmin

