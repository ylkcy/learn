#include <ntifs.h>


VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	DbgPrintEx(77, 0, "DriverUnLoad\r\n");
}

/*
typedef struct _OB_PRE_CREATE_HANDLE_INFORMATION 
{
	_Inout_ ACCESS_MASK         DesiredAccess;
	_In_ ACCESS_MASK            OriginalDesiredAccess;
} OB_PRE_CREATE_HANDLE_INFORMATION, *POB_PRE_CREATE_HANDLE_INFORMATION;
*/

/*
typedef struct _OB_PRE_DUPLICATE_HANDLE_INFORMATION 
{
	_Inout_ ACCESS_MASK         DesiredAccess;
	_In_ ACCESS_MASK            OriginalDesiredAccess;
	_In_ PVOID                  SourceProcess;
	_In_ PVOID                  TargetProcess;
} OB_PRE_DUPLICATE_HANDLE_INFORMATION, *POB_PRE_DUPLICATE_HANDLE_INFORMATION;
*/

/*
*typedef struct _OB_PRE_OPERATION_INFORMATION 
*{
*   _In_ OB_OPERATION					Operation;                  //�����Ĳ���
*    union 
*	{
*        _In_ ULONG Flags;
*        struct {
*            _In_ ULONG KernelHandle:1;
*            _In_ ULONG Reserved:31;
*        };
*    };
*    _In_ PVOID                         Object;                    //�򿪻��߸��ƵĶ���
*    _In_ POBJECT_TYPE                  ObjectType;                
*    _Out_ PVOID                        CallContext;
*    _In_ POB_PRE_OPERATION_PARAMETERS  Parameters;
*}OB_PRE_OPERATION_INFORMATION, *POB_PRE_OPERATION_INFORMATION;
*/

OB_PREOP_CALLBACK_STATUS ObPreOperationCallBack(_In_ PVOID RegistrationContext, _Inout_ POB_PRE_OPERATION_INFORMATION OperationInformation)
{

}

/*
*typedef struct _OB_POST_OPERATION_INFORMATION 
*{
*	_In_ OB_OPERATION  Operation;
*    union {
*        _In_ ULONG Flags;
*        struct {
*            _In_ ULONG KernelHandle:1;
*            _In_ ULONG Reserved:31;
*        };
*    };
*    _In_ PVOID                          Object;
*    _In_ POBJECT_TYPE                   ObjectType;
*    _In_ PVOID                          CallContext;
*    _In_ NTSTATUS                       ReturnStatus;
*    _In_ POB_POST_OPERATION_PARAMETERS  Parameters;
*} OB_POST_OPERATION_INFORMATION,*POB_POST_OPERATION_INFORMATION;
*/

VOID ObPostOperationCallBack(_In_ PVOID RegistrationContext, _In_ POB_POST_OPERATION_INFORMATION OperationInformation)
{

}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pReg)
{
	NTSTATUS status = STATUS_SUCCESS;

	pDriverObject->DriverUnload = DriverUnload;
   /*
	*typedef struct _OB_OPERATION_REGISTRATION 
	*{
	*	_In_ POBJECT_TYPE                *ObjectType;                 //ָ�򴥷��ص����̵Ķ������͵�ָ��,PsProcessType,PsThreadType
    *	_In_ OB_OPERATION                Operations;                  //OB_OPERATION_HANDLE_CREATE,OB_OPERATION_HANDLE_DUPLICATE 
    *	_In_ POB_PRE_OPERATION_CALLBACK  PreOperation;                //������֮ǰ����
    *	_In_ POB_POST_OPERATION_CALLBACK PostOperation;               //������֮�����
	*}OB_OPERATION_REGISTRATION, *POB_OPERATION_REGISTRATION
	*typedef struct _OB_CALLBACK_REGISTRATION 
	*{
    *	_In_ USHORT                     Version;                      //����Ķ���ص�ע��İ汾����������Ӧָ�� OB_FLT_REGISTRATION_VERSION
    *	_In_ USHORT                     OperationRegistrationCount;   //OperationRegistration�����е���Ŀ��,��������ע��
    *	_In_ UNICODE_STRING             Altitude;                     //�ȼ��߶�, ���е���ֵ�߶ȵĹ������������򱻼��ص����нϸ���ֵ�Ĺ��������������·���I/O��ջ��
	                                                                  //����ڱ���������������Ҫ�������������ֵ��ҪС�ڱ�������
    *	_In_ PVOID                      RegistrationContext;          //ע�����������Ϣ������
    *	_In_ OB_OPERATION_REGISTRATION  *OperationRegistration;
	*} OB_CALLBACK_REGISTRATION, *POB_CALLBACK_REGISTRATION;
	*/


	return status;
}