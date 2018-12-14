#ifndef SAMEFILESMODEL_H
#define SAMEFILESMODEL_H

#include <hashingworker.h>

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include <QAbstractItemModel>
#include <QByteArray>
#include <QVector>
#include <QMap>
#include <QThread>

class SameFilesModel :public QAbstractItemModel
{
    Q_OBJECT
public:
    SameFilesModel();
    ~SameFilesModel();

    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

public slots:
    void add_file(Node* file);
    void no_more_files();
    void start_scan(QString const& directory);
    void stop_scan();
    void delete_file(QModelIndex const& index);
    void delete_same(QModelIndex const& index);

signals:
    void scan_directory(QString const& directory);
    void scan_ended(int files_scanned);
    void scan_update(int files_scanned);
    void calculate_hash(Node* file);

private:
    QMap<QByteArray, int> hash_to_id;
    QMap<qint64, Node*> size_to_ptr;
    QVector<Node*> grouped_files;
    QThread worker_thread;
    HashingWorker* worker;

    Node* unique_group;
    QMap<QByteArray, int> unique_id;

    int total_files;
    int rehashing_files;
    bool notify_ended;

    Node* get_and_remove_file_from_unique(QMap<QByteArray, int>::iterator const&);
    void add_file_to_group(Node* file, Node* group, int parent_pos);
};

#endif // SAMEFILESMODEL_H
