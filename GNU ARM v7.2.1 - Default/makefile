################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/ble/subdir.mk
-include src/subdir.mk
-include protocol/bluetooth/lib/EFR32BG12P/GCC/subdir.mk
-include protocol/bluetooth/ble_stack/src/soc/subdir.mk
-include platform/micrium_os/ports/source/gnu/subdir.mk
-include platform/micrium_os/ports/source/generic/subdir.mk
-include platform/micrium_os/kernel/source/subdir.mk
-include platform/micrium_os/cpu/source/subdir.mk
-include platform/micrium_os/common/source/rtos/subdir.mk
-include platform/micrium_os/common/source/platform_mgr/subdir.mk
-include platform/micrium_os/common/source/lib/subdir.mk
-include platform/micrium_os/common/source/kal/subdir.mk
-include platform/micrium_os/common/source/common/subdir.mk
-include platform/micrium_os/common/source/collections/subdir.mk
-include platform/micrium_os/bsp/siliconlabs/generic/source/subdir.mk
-include platform/emlib/src/subdir.mk
-include platform/emdrv/sleep/src/subdir.mk
-include platform/emdrv/rtcdrv/src/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG12P/Source/GCC/subdir.mk
-include platform/Device/SiliconLabs/EFR32BG12P/Source/subdir.mk
-include platform/subdir.mk
-include mbedtls/targets/TARGET_Silicon_Labs/subdir.mk
-include mbedtls/targets/subdir.mk
-include mbedtls/src/subdir.mk
-include mbedtls/platform/src/subdir.mk
-include mbedtls/mbed-crypto/src/subdir.mk
-include mbedtls/mbed-crypto/platform/COMPONENT_PSA_SRV_IMPL/subdir.mk
-include lorawan/system/subdir.mk
-include lorawan/lorastack/phy/subdir.mk
-include lorawan/lorastack/mac/subdir.mk
-include lorawan/subdir.mk
-include lora_rf_drivers/SX126X/subdir.mk
-include lcd-graphics/subdir.mk
-include hardware/kit/common/drivers/subdir.mk
-include hardware/kit/common/bsp/subdir.mk
-include events/equeue/subdir.mk
-include events/subdir.mk
-include emlib/subdir.mk
-include app/bluetooth/common/util/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: BLE_LoRa.axf

# Tool invocations
BLE_LoRa.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GNU ARM C++ Linker'
	arm-none-eabi-g++ -g3 -gdwarf-2 -mcpu=cortex-m4 -mthumb -T "C:\Users\ankush.kapile\SimplicityStudio\v4_workspace\BLE_LoRa\efr32bg12p332f1024gl125.ld" --specs=nosys.specs -Xlinker --gc-sections -Xlinker -Map="BLE_LoRa.map" -mfpu=fpv4-sp-d16 -mfloat-abi=softfp --specs=nano.specs -o BLE_LoRa.axf "./src/ble/app.o" "./src/ble/app_master.o" "./src/ble/app_slave.o" "./src/ble/app_utils.o" "./src/pg_retargetswo.o" "./protocol/bluetooth/ble_stack/src/soc/rtos_bluetooth.o" "./platform/micrium_os/ports/source/gnu/armv7m_cpu_a.o" "./platform/micrium_os/ports/source/gnu/armv7m_cpu_c.o" "./platform/micrium_os/ports/source/gnu/armv7m_os_cpu_a.o" "./platform/micrium_os/ports/source/gnu/armv7m_os_cpu_c.o" "./platform/micrium_os/ports/source/generic/armv6m_v7m_cpu_int.o" "./platform/micrium_os/kernel/source/os_cfg_app.o" "./platform/micrium_os/kernel/source/os_core.o" "./platform/micrium_os/kernel/source/os_dbg.o" "./platform/micrium_os/kernel/source/os_flag.o" "./platform/micrium_os/kernel/source/os_mem.o" "./platform/micrium_os/kernel/source/os_mon.o" "./platform/micrium_os/kernel/source/os_msg.o" "./platform/micrium_os/kernel/source/os_mutex.o" "./platform/micrium_os/kernel/source/os_prio.o" "./platform/micrium_os/kernel/source/os_q.o" "./platform/micrium_os/kernel/source/os_sem.o" "./platform/micrium_os/kernel/source/os_stat.o" "./platform/micrium_os/kernel/source/os_task.o" "./platform/micrium_os/kernel/source/os_tick.o" "./platform/micrium_os/kernel/source/os_time.o" "./platform/micrium_os/kernel/source/os_tmr.o" "./platform/micrium_os/kernel/source/os_var.o" "./platform/micrium_os/cpu/source/cpu_core.o" "./platform/micrium_os/common/source/rtos/rtos_err_str.o" "./platform/micrium_os/common/source/platform_mgr/platform_mgr.o" "./platform/micrium_os/common/source/lib/lib_ascii.o" "./platform/micrium_os/common/source/lib/lib_math.o" "./platform/micrium_os/common/source/lib/lib_mem.o" "./platform/micrium_os/common/source/lib/lib_str.o" "./platform/micrium_os/common/source/kal/kal_kernel.o" "./platform/micrium_os/common/source/common/common.o" "./platform/micrium_os/common/source/collections/bitmap.o" "./platform/micrium_os/common/source/collections/map.o" "./platform/micrium_os/common/source/collections/slist.o" "./platform/micrium_os/bsp/siliconlabs/generic/source/bsp_cpu.o" "./platform/micrium_os/bsp/siliconlabs/generic/source/bsp_os.o" "./platform/micrium_os/bsp/siliconlabs/generic/source/bsp_tick_rtcc.o" "./platform/micrium_os/bsp/siliconlabs/generic/source/bsp_trace.o" "./platform/emlib/src/em_assert.o" "./platform/emlib/src/em_burtc.o" "./platform/emlib/src/em_cmu.o" "./platform/emlib/src/em_core.o" "./platform/emlib/src/em_cryotimer.o" "./platform/emlib/src/em_crypto.o" "./platform/emlib/src/em_emu.o" "./platform/emlib/src/em_eusart.o" "./platform/emlib/src/em_gpio.o" "./platform/emlib/src/em_i2c.o" "./platform/emlib/src/em_msc.o" "./platform/emlib/src/em_rmu.o" "./platform/emlib/src/em_rtcc.o" "./platform/emlib/src/em_se.o" "./platform/emlib/src/em_system.o" "./platform/emlib/src/em_timer.o" "./platform/emlib/src/em_usart.o" "./platform/emdrv/sleep/src/sleep.o" "./platform/emdrv/rtcdrv/src/rtcdriver.o" "./platform/Device/SiliconLabs/EFR32BG12P/Source/GCC/startup_efr32bg12p.o" "./platform/Device/SiliconLabs/EFR32BG12P/Source/system_efr32bg12p.o" "./platform/SPI.o" "./platform/mbed_critical.o" "./mbedtls/targets/TARGET_Silicon_Labs/aes_aes.o" "./mbedtls/targets/TARGET_Silicon_Labs/crypto_aes.o" "./mbedtls/targets/TARGET_Silicon_Labs/crypto_ecp.o" "./mbedtls/targets/TARGET_Silicon_Labs/crypto_management.o" "./mbedtls/targets/TARGET_Silicon_Labs/crypto_sha.o" "./mbedtls/targets/hash_wrappers.o" "./mbedtls/src/certs.o" "./mbedtls/src/debug.o" "./mbedtls/src/error.o" "./mbedtls/src/net_sockets.o" "./mbedtls/src/pkcs11.o" "./mbedtls/src/ssl_cache.o" "./mbedtls/src/ssl_ciphersuites.o" "./mbedtls/src/ssl_cli.o" "./mbedtls/src/ssl_cookie.o" "./mbedtls/src/ssl_srv.o" "./mbedtls/src/ssl_ticket.o" "./mbedtls/src/ssl_tls.o" "./mbedtls/src/version.o" "./mbedtls/src/version_features.o" "./mbedtls/src/x509.o" "./mbedtls/src/x509_create.o" "./mbedtls/src/x509_crl.o" "./mbedtls/src/x509_crt.o" "./mbedtls/src/x509_csr.o" "./mbedtls/src/x509write_crt.o" "./mbedtls/src/x509write_csr.o" "./mbedtls/platform/src/mbed_trng.o" "./mbedtls/platform/src/platform_alt.o" "./mbedtls/mbed-crypto/src/aes.o" "./mbedtls/mbed-crypto/src/aesni.o" "./mbedtls/mbed-crypto/src/arc4.o" "./mbedtls/mbed-crypto/src/aria.o" "./mbedtls/mbed-crypto/src/asn1parse.o" "./mbedtls/mbed-crypto/src/asn1write.o" "./mbedtls/mbed-crypto/src/base64.o" "./mbedtls/mbed-crypto/src/bignum.o" "./mbedtls/mbed-crypto/src/blowfish.o" "./mbedtls/mbed-crypto/src/camellia.o" "./mbedtls/mbed-crypto/src/ccm.o" "./mbedtls/mbed-crypto/src/chacha20.o" "./mbedtls/mbed-crypto/src/chachapoly.o" "./mbedtls/mbed-crypto/src/cipher.o" "./mbedtls/mbed-crypto/src/cipher_wrap.o" "./mbedtls/mbed-crypto/src/cmac.o" "./mbedtls/mbed-crypto/src/ctr_drbg.o" "./mbedtls/mbed-crypto/src/des.o" "./mbedtls/mbed-crypto/src/dhm.o" "./mbedtls/mbed-crypto/src/ecdh.o" "./mbedtls/mbed-crypto/src/ecdsa.o" "./mbedtls/mbed-crypto/src/ecjpake.o" "./mbedtls/mbed-crypto/src/ecp.o" "./mbedtls/mbed-crypto/src/ecp_curves.o" "./mbedtls/mbed-crypto/src/entropy.o" "./mbedtls/mbed-crypto/src/entropy_poll.o" "./mbedtls/mbed-crypto/src/gcm.o" "./mbedtls/mbed-crypto/src/havege.o" "./mbedtls/mbed-crypto/src/hkdf.o" "./mbedtls/mbed-crypto/src/hmac_drbg.o" "./mbedtls/mbed-crypto/src/md.o" "./mbedtls/mbed-crypto/src/md2.o" "./mbedtls/mbed-crypto/src/md4.o" "./mbedtls/mbed-crypto/src/md5.o" "./mbedtls/mbed-crypto/src/md_wrap.o" "./mbedtls/mbed-crypto/src/memory_buffer_alloc.o" "./mbedtls/mbed-crypto/src/nist_kw.o" "./mbedtls/mbed-crypto/src/oid.o" "./mbedtls/mbed-crypto/src/padlock.o" "./mbedtls/mbed-crypto/src/pem.o" "./mbedtls/mbed-crypto/src/pk.o" "./mbedtls/mbed-crypto/src/pk_wrap.o" "./mbedtls/mbed-crypto/src/pkcs12.o" "./mbedtls/mbed-crypto/src/pkcs5.o" "./mbedtls/mbed-crypto/src/pkparse.o" "./mbedtls/mbed-crypto/src/pkwrite.o" "./mbedtls/mbed-crypto/src/platform.o" "./mbedtls/mbed-crypto/src/platform_util.o" "./mbedtls/mbed-crypto/src/poly1305.o" "./mbedtls/mbed-crypto/src/ripemd160.o" "./mbedtls/mbed-crypto/src/rsa.o" "./mbedtls/mbed-crypto/src/rsa_internal.o" "./mbedtls/mbed-crypto/src/sha1.o" "./mbedtls/mbed-crypto/src/sha256.o" "./mbedtls/mbed-crypto/src/sha512.o" "./mbedtls/mbed-crypto/src/threading.o" "./mbedtls/mbed-crypto/src/timing.o" "./mbedtls/mbed-crypto/src/xtea.o" "./mbedtls/mbed-crypto/platform/COMPONENT_PSA_SRV_IMPL/psa_crypto.o" "./mbedtls/mbed-crypto/platform/COMPONENT_PSA_SRV_IMPL/psa_crypto_slot_management.o" "./mbedtls/mbed-crypto/platform/COMPONENT_PSA_SRV_IMPL/psa_crypto_storage.o" "./mbedtls/mbed-crypto/platform/COMPONENT_PSA_SRV_IMPL/psa_its_file.o" "./lorawan/system/LoRaWANTimer.o" "./lorawan/lorastack/phy/LoRaPHY.o" "./lorawan/lorastack/phy/LoRaPHYAS923.o" "./lorawan/lorastack/phy/LoRaPHYAU915.o" "./lorawan/lorastack/phy/LoRaPHYCN470.o" "./lorawan/lorastack/phy/LoRaPHYCN779.o" "./lorawan/lorastack/phy/LoRaPHYEU433.o" "./lorawan/lorastack/phy/LoRaPHYEU868.o" "./lorawan/lorastack/phy/LoRaPHYIN865.o" "./lorawan/lorastack/phy/LoRaPHYKR920.o" "./lorawan/lorastack/phy/LoRaPHYUS915.o" "./lorawan/lorastack/mac/LoRaMac.o" "./lorawan/lorastack/mac/LoRaMacChannelPlan.o" "./lorawan/lorastack/mac/LoRaMacCommand.o" "./lorawan/lorastack/mac/LoRaMacCrypto.o" "./lorawan/LoRaWANInterface.o" "./lorawan/LoRaWANStack.o" "./lora_rf_drivers/SX126X/SX126X_LoRaRadio.o" "./lcd-graphics/bmp.o" "./lcd-graphics/display.o" "./lcd-graphics/displayls013b7dh03.o" "./lcd-graphics/displaypalemlib.o" "./lcd-graphics/dmd_display.o" "./lcd-graphics/glib.o" "./lcd-graphics/glib_bitmap.o" "./lcd-graphics/glib_circle.o" "./lcd-graphics/glib_font_narrow_6x8.o" "./lcd-graphics/glib_font_normal_8x8.o" "./lcd-graphics/glib_font_number_16x20.o" "./lcd-graphics/glib_line.o" "./lcd-graphics/glib_polygon.o" "./lcd-graphics/glib_rectangle.o" "./lcd-graphics/glib_string.o" "./lcd-graphics/graphics.o" "./lcd-graphics/udelay.o" "./hardware/kit/common/drivers/mx25flash_spi.o" "./hardware/kit/common/bsp/bsp_stk.o" "./hardware/kit/common/bsp/bsp_stk_leds.o" "./events/equeue/equeue.o" "./events/equeue/equeue_mbed.o" "./events/equeue/equeue_posix.o" "./events/EventQueue.o" "./events/mbed_shared_queues.o" "./emlib/em_ldma.o" "./app/bluetooth/common/util/infrastructure.o" "./application_properties.o" "./ble_main.o" "./dmadrv.o" "./gatt_db.o" "./gpiointerrupt.o" "./init_app.o" "./init_board.o" "./init_mcu.o" "./main.o" "./pti.o" "./spidrv.o" "C:\Users\ankush.kapile\SimplicityStudio\v4_workspace\BLE_LoRa\protocol\bluetooth\lib\EFR32BG12P\GCC\libbluetooth.a" "C:\Users\ankush.kapile\SimplicityStudio\v4_workspace\BLE_LoRa\protocol\bluetooth\lib\EFR32BG12P\GCC\libpsstore.a" "C:\Users\ankush.kapile\SimplicityStudio\v4_workspace\BLE_LoRa\protocol\bluetooth\lib\EFR32BG12P\GCC\libmbedtls.a" "C:\Users\ankush.kapile\SimplicityStudio\v4_workspace\BLE_LoRa\protocol\bluetooth\lib\EFR32BG12P\GCC\binapploader.o" "C:\Users\ankush.kapile\SimplicityStudio\v4_workspace\BLE_LoRa\platform\radio\rail_lib\autogen\librail_release\librail_efr32xg12_gcc_release.a" -lm "./retargetio.o" -Wl,--start-group -lgcc -lc -lnosys -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

	@echo 'Building hex file: BLE_LoRa.hex'
	arm-none-eabi-objcopy -O ihex "BLE_LoRa.axf" "BLE_LoRa.hex"
	@echo ' '

	@echo 'Building bin file: BLE_LoRa.bin'
	arm-none-eabi-objcopy -O binary "BLE_LoRa.axf" "BLE_LoRa.bin"
	@echo ' '

	@echo 'Building s37 file: BLE_LoRa.s37'
	arm-none-eabi-objcopy -O srec "BLE_LoRa.axf" "BLE_LoRa.s37"
	@echo ' '

	@echo 'Running size tool'
	arm-none-eabi-size "BLE_LoRa.axf"
	@echo ' '

# Other Targets
clean:
	-$(RM) $(CC_DEPS)$(C++_DEPS)$(EXECUTABLES)$(OBJS)$(C_UPPER_DEPS)$(CXX_DEPS)$(C_DEPS)$(CPP_DEPS) BLE_LoRa.axf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
