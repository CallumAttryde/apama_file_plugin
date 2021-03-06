FROM kpalf/apamacore:10.3.0.1_ubuntu_amd64 as builder
COPY . ${APAMA_WORK}/apama_file_plugin
USER root
# xpybuild is python2 but pysys is python3, argh!
RUN apt-get update && apt-get install -y g++ git-core python python3
RUN cd ${APAMA_WORK}/apama_file_plugin && git submodule init && git submodule update
RUN cd ${APAMA_WORK}/apama_file_plugin && python xpybuild/xpybuild.py;
RUN cd ${APAMA_WORK}/apama_file_plugin/tests; pysys run | tee logfile && grep 'THERE WERE NO NON PASSES' logfile

FROM kpalf/apamacore:10.3.0.1_ubuntu_amd64
COPY --from=builder ${APAMA_WORK}/lib ${APAMA_WORK}/lib
COPY --from=builder ${APAMA_WORK}/monitors ${APAMA_WORK}/monitors
USER sagadmin:sagadmin
