#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/jiffies.h>
#include <linux/mutex.h>

#define JIFFIES_MAX_LEN 11

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple debug driver");
MODULE_AUTHOR("Joakim Lönnegren");

static const char *eudyptula_id = "a005a48651a3\n\0";
static char k_input[256] = {0};
static char jiffies_buffer[JIFFIES_MAX_LEN] = {0};
static char foo_data[PAGE_SIZE] = {0};

static struct dentry *eudyptula_dir;

static DEFINE_MUTEX(foo_mutex);

ssize_t id_write(struct file *f, const char *input, size_t count,
		loff_t *position)
{
	ssize_t ret = simple_write_to_buffer(k_input, sizeof(k_input),
			position, input, count);
	if (ret < 0)
		return ret;

	if (strncmp(eudyptula_id, k_input, strlen(eudyptula_id)))
		return -EINVAL;

	return ret;
}

ssize_t id_read(struct file *f, char *user_buffer, size_t count,
		loff_t *position)
{
	return simple_read_from_buffer(user_buffer, count, position,
			eudyptula_id, strlen(eudyptula_id));
}

static const struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.write = id_write,
	.read = id_read,
};

ssize_t jiffies_read(struct file *f, char *user_buffer, size_t count,
                loff_t *position)
{
        int ret = snprintf(jiffies_buffer, JIFFIES_MAX_LEN, "%lu", jiffies);
        if (ret != JIFFIES_MAX_LEN-1)
                return ret;

        return simple_read_from_buffer(user_buffer, count, position,
                        jiffies_buffer, strlen(jiffies_buffer));
}

static const struct file_operations jiffies_fops = {
        .owner = THIS_MODULE,
        .read = jiffies_read,
};

ssize_t foo_write(struct file *f, const char *input, size_t count,
		loff_t *position)
{
	ssize_t ret = 0;

	mutex_lock_interruptible(&foo_mutex);
	ret = simple_write_to_buffer(foo_data, sizeof(foo_data),
			position, input, count);
	mutex_unlock(&foo_mutex);

	return ret;
}

ssize_t foo_read(struct file *f, char *user_buffer, size_t count,
		loff_t *position)
{
	mutex_lock_interruptible(&foo_mutex);
	int ret = simple_read_from_buffer(user_buffer, count, position,
			foo_data, strlen(foo_data));
	mutex_unlock(&foo_mutex);

	return ret;
}

static const struct file_operations foo_fops = {
        .owner = THIS_MODULE,
        .read = foo_read,
	.write = foo_write,
};

static int simple_init(void)
{
	int ret = 0;

	eudyptula_dir = debugfs_create_dir("eudyptula", NULL);
	if (eudyptula_dir < 0) {
		ret = eudyptula_dir;
		goto err_return;
	}

	ret = debugfs_create_file("id", 0666, eudyptula_dir,
			NULL, &id_fops);
	if (ret < 0)
		goto err_remove_dir;

	ret = debugfs_create_file("jiffies", 0444, eudyptula_dir,
                        NULL, &jiffies_fops);
	if (ret < 0)
		goto err_remove_dir;

	ret = debugfs_create_file("foo", 0644, eudyptula_dir,
                        foo_data, &foo_fops);
	if (ret < 0)
		goto err_remove_dir;

	return 0;
err_remove_dir:
	debugfs_remove_recursive(eudyptula_dir);
err_return:
	return ret;
}

static void simple_exit(void)
{
	debugfs_remove_recursive(eudyptula_dir);
}

module_init(simple_init);
module_exit(simple_exit);
