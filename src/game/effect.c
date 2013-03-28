#include "game.h"
#include "common.h"
#include "string.h"
#include "adt/linklist.h"
#include "device/video.h"
#include "x86/x86.h"

LINKLIST_IMPL(fly, 10000)

static fly_t head = NULL;
static int hit = 0, miss = 0;

int
get_hit(void) {
	return hit;
}

int
get_miss(void) {
	return miss;
}

fly_t
characters(void) {
	return head;
}
/* 在屏幕上创建主要角色 */
void
create_main_character(void){
	ME.x = SCR_HEIGHT / 2;
	ME.y = SCR_WIDTH / 2;
	ME.di = 2;
}
/* 在屏幕上创建一个新的敌人 */
void
create_new_enemy(void) {
	if (head == NULL) {
		head = fly_new(); /* 当前没有任何字母，创建新链表 */
	} else {
		fly_t now = fly_new();
		fly_insert(NULL, head, now); /* 插入到链表的头部 */
		head = now;
	}
	/* 字母、初始位置、掉落速度均为随机设定 */
	head->x = 0;
	head->y = rand() % (SCR_WIDTH / 8 - 2) * 8 + 8;
	//head->v = (rand() % 1000 / 1000.0 + 1) / 2.0;
	head->di = rand() % 4;
	//release_key(head->text); /* 清除过往的按键 */
}

/* 敌人移动1单位 */
void
update_enemy_pos(void) {
	fly_t it;
	for (it = head; it != NULL; ) {
		fly_t next = it->_next;
		//it->x += it->v; /* 根据速度更新位置 */
		if (it->x < 0 || it->x + 7.9 > SCR_HEIGHT) {
			if (it->x < 0) hit ++; /* 从上部飞出屏幕 */
			else miss ++; /* 从下部飞出屏幕 */
			fly_remove(it);
			fly_free(it);
			if (it == head) head = next; /* 更新链表 */
		}
		it = next;
	}
}

/* 根据按键移动主角的位置 */
bool
update_keypress(void) {
	//fly_t it, target = NULL;
	//float min = -100;
	
	disable_interrupt();
	/* 寻找相应键已被按下、最底部且未被击中的字符 
	for (it = head; it != NULL; it = it->_next) {
		assert(it->text >= 0 && it->text < 26);
		if (it->v > 0 && it->x > min && query_key(it->text)) {
			min = it->x;
			target = it;
		}
	}
	//如果找到则更新相应数据 
	if (target != NULL) {
		release_key(target->text);
		target->v = -3;  //速度改为向上 
		return TRUE;
	}*/

	if(query_key(0))
	{
		if(ME.y>=SIZE_OF_CHARACTER)
			ME.y-=SIZE_OF_CHARACTER;
		ME.di = 0;
		release_key(0);
		return TRUE;
	}
	if(query_key(1))
	{
		if(ME.y<=SCR_WIDTH-SIZE_OF_CHARACTER*2)
			ME.y+=SIZE_OF_CHARACTER;
		ME.di = 1;
		release_key(1);
		return TRUE;
	}
	if(query_key(2))
	{
		if(ME.x>=SIZE_OF_CHARACTER)
			ME.x-=SIZE_OF_CHARACTER;
		ME.di = 2;
		release_key(2);
		return TRUE;
	}
	if(query_key(3))
	{
		if(ME.x<=SCR_HEIGHT-SIZE_OF_CHARACTER*2)
			ME.x+=SIZE_OF_CHARACTER;
		ME.di = 3;
		release_key(3);
		return TRUE;
	}
	enable_interrupt();
	return FALSE;
}
