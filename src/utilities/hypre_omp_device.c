#include "_hypre_utilities.h"

#if defined(HYPRE_USING_DEVICE_OPENMP)

/* global variables for device OpenMP */
HYPRE_Int hypre__global_offload = 0;
HYPRE_Int hypre__offload_device_num;
HYPRE_Int hypre__offload_host_num;

/* stats */
size_t hypre__target_allc_count = 0;
size_t hypre__target_free_count = 0;
size_t hypre__target_allc_bytes = 0;
size_t hypre__target_free_bytes = 0;

size_t hypre__target_htod_count = 0;
size_t hypre__target_dtoh_count = 0;
size_t hypre__target_htod_bytes = 0;
size_t hypre__target_dtoh_bytes = 0;

/* num: number of bytes */
HYPRE_Int HYPRE_OMPOffload(HYPRE_Int device, void *ptr, size_t num,
                           const char *type1, const char *type2) {
   hypre_omp45_offload(device, ptr, char, 0, num, type1, type2);

   return 0;
}

HYPRE_Int HYPRE_OMPPtrIsMapped(void *p, HYPRE_Int device_num)
{
   if (hypre__global_offload && !omp_target_is_present(p, device_num)) {
      printf("HYPRE mapping error: %p has not been mapped to device %d!\n", p, device_num);
      return 1;
   }
   return 0;
}

/* OMP offloading switch */
HYPRE_Int HYPRE_OMPOffloadOn()
{
   hypre__global_offload = 1;
   hypre__offload_device_num = omp_get_default_device();
   hypre__offload_host_num   = omp_get_initial_device();
   hypre_fprintf(stdout, "Hypre OMP 4.5 offloading has been turned on. Device %d\n",
                 hypre__offload_device_num);

   return 0;
}

HYPRE_Int HYPRE_OMPOffloadOff()
{
   fprintf(stdout, "Hypre OMP 4.5 offloading has been turned off\n");
   hypre__global_offload = 0;
   hypre__offload_device_num = omp_get_initial_device();
   hypre__offload_host_num   = omp_get_initial_device();

   return 0;
}

HYPRE_Int HYPRE_OMPOffloadStatPrint()
{
   hypre_printf("Hypre OMP target memory stats:\n"
                "      ALLOC   %ld bytes, %ld counts\n"
                "      FREE    %ld bytes, %ld counts\n"
                "      HTOD    %ld bytes, %ld counts\n"
                "      DTOH    %ld bytes, %ld counts\n",
                hypre__target_allc_bytes, hypre__target_allc_count,
                hypre__target_free_bytes, hypre__target_free_count,
                hypre__target_htod_bytes, hypre__target_htod_count,
                hypre__target_dtoh_bytes, hypre__target_dtoh_count);

   return 0;
}

#endif /* #if defined(HYPRE_USING_DEVICE_OPENMP) */

