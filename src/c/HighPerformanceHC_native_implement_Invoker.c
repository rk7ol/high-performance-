#include "HighPerformanceHC_native_implement_Invoker.h"
#include <stdlib.h>
#include <unistd.h>

#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#include "utils.h"

JNIEXPORT void JNICALL Java_HighPerformanceHC_native_1implement_Invoker_InvokeMPIHeatConduct(JNIEnv *env, jobject obj, jstring exec_path, jint x_size, jint y_size, jint z_size, jobjectArray value, jint process_number)
{

    pid_t pid;

    const char *fifo_name = "tmp";
    int fid;

   if (access(fifo_name, F_OK) == -1)
   {
        if (mkfifo(fifo_name, 0644) != 0)
        {
            fprintf(stderr, "Could not create fifo %s\n", fifo_name);
           // return;
        }
   }

    pid = fork();

    if (pid == 0)
    {

        printf("child : %d\n", getpid());

        int result[x_size][y_size][z_size];

        int x_array_length = (*env)->GetArrayLength(env, value);

        for (size_t i = 0; i < x_array_length; i++)
        {
            jarray x_array = (*env)->GetObjectArrayElement(env, value, i);

            int xy_array_length = (*env)->GetArrayLength(env, x_array);

            for (size_t j = 0; j < xy_array_length; j++)
            {
                jarray xy_array = (*env)->GetObjectArrayElement(env, x_array, j);

                int xyz_array_length = (*env)->GetArrayLength(env, xy_array);

                for (size_t k = 0; k < xyz_array_length; k++)
                {
                    jint *int_value_array = (*env)->GetIntArrayElements(env, xy_array, NULL);

                    result[i][j][k] = int_value_array[k];
                }
            }
        }

        fid = open("tmp", O_WRONLY);

        write(fid, &result, sizeof(int) * x_size * y_size * z_size);

        close(fid);

        return;
    }

    printf("parent : %d\n", getpid());

    char **args = malloc(sizeof(char *) * 9);

    args[0] = "mpirun";

    args[1] = "-np";

    char *process_number_string = int2string(process_number);

    args[2] = process_number_string;

    const char *exec_path_c = (*env)->GetStringUTFChars(env, exec_path, NULL);

    args[3] = (char *)exec_path_c;

    args[4] = (char *)fifo_name;

    args[5] = int2string(x_size);

    args[6] = int2string(y_size);

    args[7] = int2string(z_size);

    args[8] = NULL;

    if (execvp("mpirun", args) == -1)
    {
        fprintf(stdout, "failed errno:%d\n", errno);

        exit(errno);
    }

    return;
}