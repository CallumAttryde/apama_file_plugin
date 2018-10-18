from pysys.constants import *
from apama.correlator import CorrelatorHelper
from pysys.basetest import BaseTest

class PySysTest(BaseTest):
	def execute(self):
		self.correlator = CorrelatorHelper(self)
		self.correlator.start(environ={'APAMA_FILEPLUGIN_ROOT_DIR':self.output})
		self.correlator.injectEPL(filenames=['FilePlugin.mon'], filedir=PROJECT.APAMA_WORK + '/monitors')
		self.correlator.injectMonitorscript(filenames=['test.mon'])
		self.waitForSignal('correlator.out', expr='Test finished', errorExpr=[' (ERROR|FATAL) '])

	def validate(self):
		self.assertGrep('correlator.out', expr=' (ERROR|FATAL) ', contains=False)
		self.assertGrep('correlator.out', expr='cannot read.*missing_file.out.*in plugin method read')
		self.assertGrep('correlator.out', expr='cannot get file size.*missing_file.out.*in plugin method get_file_size_MB')
		self.assertGrep('correlator.out', expr='cannot copy.*missing_file.out.*in plugin method copy') 
		self.assertGrep('correlator.out', expr='cannot remove.*missing_file.out.*in plugin method remove')
