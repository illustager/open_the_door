#ifndef USER_DATA_H
#define USER_DATA_H

namespace user_data {
	const byte user_num = 3;

	struct user {
		char name[10];
		uint32_t id;
	};

	struct user all_users[user_num] = {
		{"唐宇坤", 0xDED0135F},
		{"杨东霖", 0xAE2B175F},
		{"刘冰洋", 0x2EB7115F}
	};

	int check(uint32_t id) { // 匹配失败返回 -1 否则返回用户在 all_user 中的索引
		for(byte i = 0; i < user_num; ++i) {
			if(all_users[i].id == id)  return (int)i;
		}
		return -1;
	}

} // namespace user_data

#endif // USER_DATA_H
