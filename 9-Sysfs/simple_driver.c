#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/jiffies.h>
#include <linux/mutex.h>
#include <linux/sysfs.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple debug driver");
MODULE_AUTHOR("Joakim Lönnegren");

static struct kobject *eudyptula;

static const char *eudyptula_id = "a005a48651a3\n\0";
static ssize_t id_store(struct kobject *kobj, struct kobj_attribute *attr,
		const char *buf, size_t count);
static ssize_t id_show(struct kobject *kobj, struct kobj_attribute *attr,
		char *buf);
static struct kobj_attribute id_attr = __ATTR_RW(id);


static ssize_t jiffies_show(struct kobject *kobj, struct kobj_attribute *attr,
		char *buf);
static struct kobj_attribute jiffies_attr = __ATTR_RO(jiffies);


static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
		const char *buf, size_t count);
static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
		char* buf);
static struct kobj_attribute foo_attr = __ATTR_RW(foo);
static char foo_data[PAGE_SIZE] = {0};
static DEFINE_MUTEX(foo_mutex);

static ssize_t id_store(struct kobject *kobj, struct kobj_attribute *attr,
		const char *buf, size_t count)
{
	if (strncmp(eudyptula_id, buf, strlen(eudyptula_id)))
		return -EINVAL;

	return count;
}

static ssize_t id_show(struct kobject *kobj, struct kobj_attribute *attr,
		char *buf)
{
	return sprintf(buf, "%s", eudyptula_id);
}

static ssize_t jiffies_show(struct kobject *kobj, struct kobj_attribute *attr,
		char *buf)
{
	return sprintf(buf, "%lu", jiffies);
}

static ssize_t foo_store(struct kobject *kobj, struct kobj_attribute *attr,
		const char *buf, size_t count)
{
	ssize_t ret = 0;

	mutex_lock_interruptible(&foo_mutex);
	ret = sprintf(foo_data, "%s", buf);
	mutex_unlock(&foo_mutex);

	return ret;
}

static ssize_t foo_show(struct kobject *kobj, struct kobj_attribute *attr,
		char *buf)
{
	ssize_t ret = 0;

	mutex_lock_interruptible(&foo_mutex);
	ret = sprintf(buf, "%s", foo_data);
	mutex_unlock(&foo_mutex);

	return ret;
}

static int simple_init(void)
{
	int ret = 0;

	eudyptula = kobject_create_and_add("eudyptula", kernel_kobj);
	if (eudyptula < 0) {
		ret = eudyptula;
		goto err_return;
	}

	if (sysfs_create_file(eudyptula, &id_attr.attr))
		goto err_remove_sysfs;

	if (sysfs_create_file(eudyptula, &jiffies_attr.attr))
		goto err_remove_sysfs;

	if (sysfs_create_file(eudyptula, &foo_attr.attr))
		goto err_remove_sysfs;

	return 0;
err_remove_sysfs:
	kobject_put(eudyptula);
	sysfs_remove_file(kernel_kobj, &id_attr.attr);
	sysfs_remove_file(kernel_kobj, &jiffies_attr.attr);
	sysfs_remove_file(kernel_kobj, &foo_attr.attr);
err_return:
	return ret;
}

static void simple_exit(void)
{
	kobject_put(eudyptula);
	sysfs_remove_file(kernel_kobj, &id_attr.attr);
	sysfs_remove_file(kernel_kobj, &jiffies_attr.attr);
	sysfs_remove_file(kernel_kobj, &foo_attr.attr);
}

module_init(simple_init);
module_exit(simple_exit);
