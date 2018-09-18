from pysys.constants import *
from apama.correlator import CorrelatorHelper
from pysys.basetest import BaseTest

class PySysTest(BaseTest):
	def execute(self):
            self.correlator = CorrelatorHelper(self)
            self.correlator.start(environ={'APAMA_FILEPLUGIN_ROOT_DIR':self.output})
            self.correlator.injectMonitorscript(filenames=['test.mon'])
            self.waitForSignal('correlator.out', expr='Test finished', errorExpr=[' (ERROR|FATAL) '])

        def validate(self):
            self.assertGrep('correlator.out', expr=' (ERROR|FATAL) ', contains=False)
            self.assertGrep('correlator.out', expr='Could not find.*missing.*in plugin method read')
            self.assertGrep('correlator.out', expr='Could not find.*missing.*in plugin method get_file_size_MB')
            self.assertGrep('correlator.out', expr='Could not find.*missing.*in plugin method copy') 
            self.assertGrep('correlator.out', expr='Unable to remove file.*missing.*in plugin method remove')
