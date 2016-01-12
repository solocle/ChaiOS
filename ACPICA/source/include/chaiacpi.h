#ifndef CHAIOS_ACPICA_ACPI_H
#define CHAIOS_ACPICA_ACPI_H

#ifdef __cplusplus
extern "C" {
#endif
#include <acpi.h>
#ifdef __cplusplus
}
#endif

//Provide some simpler init interfaces
extern "C" ACPI_STATUS AcpiEarlyTables();
extern "C" ACPI_STATUS StartAcpi();

EXTERN CHAIOS_API_FUNC(ACPI_STATUS GetAcpiTable(char* TableName, uint32_t instance, void** table));

#endif