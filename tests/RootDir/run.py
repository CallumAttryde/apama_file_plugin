from pysys.constants import *
from apama.correlator import CorrelatorHelper
from pysys.basetest import BaseTest


class PySysTest(BaseTest):
	def execute(self):
            self.correlator_default = CorrelatorHelper(self, name='correlator_default')
            self.correlator_tailored = CorrelatorHelper(self, name='correlator_tailored')

            self.correlator_default.start()
            self.correlator_tailored.start(environ={'APAMA_FILEPLUGIN_ROOT_DIR':self.output})

            self.correlator_default.injectEPL(filenames=['FilePlugin.mon'], filedir=PROJECT.APAMA_WORK + '/monitors')
            self.correlator_tailored.injectEPL(filenames=['FilePlugin.mon'], filedir=PROJECT.APAMA_WORK + '/monitors')

            self.correlator_default.injectMonitorscript(filenames=['test.mon'])
            self.correlator_tailored.injectMonitorscript(filenames=['test.mon'])

            self.waitForSignal('correlator_default.out', expr='Test finished')
            self.waitForSignal('correlator_tailored.out', expr='Test finished')

	def validate(self):
	    self.assertGrep('correlator_default.out', expr=self.project.APAMA_WORK)
            self.assertGrep('correlator_tailored.out', expr=self.output)
