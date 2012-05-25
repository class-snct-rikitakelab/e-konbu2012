#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"
#include "balancer.h" /* �|���U�q����p�w�b�_�t�@�C�� */
#include "logSend.h"
#include "Maimai.h"
#include <stdlib.h>



float Maimai(){
static unsigned int counter_maimai=0;
U16 lightDowned;
U16 lightUpped;
U16 lightDiff;
float k;
static float brightness;
//static float brighteness,buf_brighness;

++counter_maimai;
if(counter_maimai==20/4){//��20ms
	//�ԐFLED�������̃Z���T�l���擾
	lightDowned=ecrobot_get_light_sensor(NXT_PORT_S3);
	//�ԐFLED��_������
	ecrobot_set_light_sensor_active(NXT_PORT_S3);
	}

if(counter_maimai==40/4){//��40ms
	//�ԐFLED�_�����̃Z���T�l���擾
	lightUpped=ecrobot_get_light_sensor(NXT_PORT_S3);
	//�ԐFLED����������
	ecrobot_set_light_sensor_inactive(NXT_PORT_S3);
	//�J�E���^���N���A
	counter_maimai=0;
}

//���Z���T�̕ω��ʂ��v�Z

if(lightDowned - lightUpped>0){
	lightDiff=lightDowned - lightUpped;
}
else{
	lightDiff=0U;
}

k=(1.0382E-3 * lightDowned -6.3295E-1)*lightDowned + 1.1024E+2;
brightness = (float)lightDiff/k;

return brightness;


}