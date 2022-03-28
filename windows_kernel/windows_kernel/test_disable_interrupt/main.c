#include <ntifs.h>

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	
}


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pReg)
{
	pDriverObject->DriverUnload = DriverUnload;

	return STATUS_SUCCESS;
}