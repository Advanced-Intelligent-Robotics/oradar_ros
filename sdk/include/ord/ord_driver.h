#ifndef ORD_SDK_ORD_DRIVER_H_
#define ORD_SDK_ORD_DRIVER_H_

#include "ord/ord_types.h"
#include "ord/ord_driver_net.h"
#include "ord/ord_driver_impl.h"
#include <condition_variable>
#include <thread>
#include <atomic>

namespace ord_sdk
{

  class Impl;

  class OrdDriver
  {
  public:
    OrdDriver(const SocketAddress &sensor);
    virtual ~OrdDriver();
    virtual error_t open();
    bool isOpened() const;
    virtual void close();
    void setTimeout(int timeout);

    error_t setLidarIPAddress(in_addr_t address);
    error_t setLidarNetPort(in_port_t port);
    error_t setScanSpeed(uint32_t speed);
    error_t setTailFilterLevel(uint32_t level);
    error_t setTimestamp(uint32_t timestamp);
    error_t setScanDirection(uint32_t direction);
    error_t trackConnect();
    error_t enableMeasure();
    error_t disableMeasure();
    error_t enabelDataStream();
    error_t disableDataStream();
    error_t applyConfigs();
    error_t deviceReboot();

    error_t getLidarIPAddress(in_addr_t &address);
    error_t getLidarNetPort(in_port_t &port);
    error_t getScanSpeed(uint32_t &speed);
    error_t getTailFilterLevel(uint32_t &level);
    error_t getTimestamp(uint32_t &timestamp);
    error_t getMotorSpeed(uint32_t &motor_speed);
    error_t getInternalTemperature(float &inter_temp);
    error_t getSyncStatus(uint32_t &sync_status);
    error_t getFirmwareVersion(std::string &firmware_version);
    error_t getHardwareVersion(std::string &hardware_version);
    error_t getSDKVersion(std::string &sdk_version);
    error_t getScanDirection(uint32_t &direction);

    error_t GrabOneScan(ScanBlockData &scan_block_data);
    error_t GrabOneScanBlocking(ScanBlockData &scan_block_data);
    error_t GrabFullScan(ScanFrameData &scan_frame_data);
    error_t GrabFullScanBlocking(ScanFrameData &scan_frame_data);

  protected:
    std::unique_ptr<SocketAddress> sensor_;
    std::unique_ptr<Impl> impl_;

  private:

    std::thread *ParseThread_;
    static void ParseThread(void *param);
    std::atomic<bool> thread_exit_flag_;

    ScanFrameData scan_frame_data_;
    ScanBlockData scan_block_data_;
    std::mutex frame_mutex_;
    std::mutex block_mutex_;
    std::mutex frame_wait_mutex_;
    std::mutex block_wait_mutex_;
    std::condition_variable frame_data_cv_;
    std::condition_variable block_data_cv_;
    bool is_frame_ready_;
    bool is_block_ready_;
    uint8_t frame_begin_;

    uint32_t FilterLevel_;
    uint32_t MotorSpeed_;
    uint32_t ScanDiret_;
    uint8_t FirstGetInfo_;
    static const int FILTER_WINDOW_SIZE = 7; // 必须为奇数
    void point_cloud_filter(uint32_t FilterLevel, uint32_t MotorSpeed, std::vector<uint16_t> &distances);
  };

}

#endif
