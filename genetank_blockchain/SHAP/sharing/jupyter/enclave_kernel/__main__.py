from ipykernel.kernelapp import IPKernelApp
from .kernel import EnclaveKernel
IPKernelApp.launch_instance(kernel_class=EnclaveKernel)
