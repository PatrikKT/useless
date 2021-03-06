/* Copyright (c) 2011-2012, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#ifndef _SPS_H_
#define _SPS_H_

#include <linux/types.h>	

#define SPS_DEV_HANDLE_MEM       ((u32)0x7ffffffful)


#define SPS_DEV_HANDLE_INVALID   ((u32)0)

#define SPS_IRQ_INVALID          0

#define SPS_ADDR_INVALID      0

#define SPS_CLASS_INVALID     ((u32)-1)

#define SPS_CONFIG_DEFAULT       0

#define SPS_DMA_THRESHOLD_DEFAULT   0

#define SPS_IOVEC_FLAG_INT  0x8000  
#define SPS_IOVEC_FLAG_EOT  0x4000  
#define SPS_IOVEC_FLAG_EOB  0x2000  
#define SPS_IOVEC_FLAG_NWD  0x1000  
#define SPS_IOVEC_FLAG_CMD  0x0800  
#define SPS_IOVEC_FLAG_LOCK  0x0400  
#define SPS_IOVEC_FLAG_UNLOCK  0x0200  
#define SPS_IOVEC_FLAG_IMME 0x0100  
#define SPS_IOVEC_FLAG_NO_SUBMIT 0x0002  
#define SPS_IOVEC_FLAG_DEFAULT   0x0001  


#define SPS_BAM_OPT_ENABLE_AT_BOOT  1UL
#define SPS_BAM_OPT_IRQ_DISABLED    (1UL << 1)
#define SPS_BAM_OPT_BAMDMA          (1UL << 2)
#define SPS_BAM_OPT_IRQ_WAKEUP      (1UL << 3)


#define SPS_BAM_MGR_DEVICE_REMOTE   1UL
#define SPS_BAM_MGR_MULTI_EE        (1UL << 1)
#define SPS_BAM_MGR_PIPE_NO_ALLOC   (1UL << 2)
#define SPS_BAM_MGR_PIPE_NO_CONFIG  (1UL << 3)
#define SPS_BAM_MGR_PIPE_NO_CTRL    (1UL << 4)
#define SPS_BAM_MGR_NONE            \
	(SPS_BAM_MGR_DEVICE_REMOTE | SPS_BAM_MGR_PIPE_NO_ALLOC | \
	 SPS_BAM_MGR_PIPE_NO_CONFIG | SPS_BAM_MGR_PIPE_NO_CTRL)
#define SPS_BAM_MGR_LOCAL           0
#define SPS_BAM_MGR_LOCAL_SHARED    SPS_BAM_MGR_MULTI_EE
#define SPS_BAM_MGR_REMOTE_SHARED   \
	(SPS_BAM_MGR_DEVICE_REMOTE | SPS_BAM_MGR_MULTI_EE | \
	 SPS_BAM_MGR_PIPE_NO_ALLOC)
#define SPS_BAM_MGR_ACCESS_MASK     SPS_BAM_MGR_NONE

#define SPS_BAM_NUM_EES             4
#define SPS_BAM_SEC_DO_NOT_CONFIG   0
#define SPS_BAM_SEC_DO_CONFIG       0x0A434553

enum sps_mode {
	SPS_MODE_SRC = 0,  
	SPS_MODE_DEST,	   
};


enum sps_option {
	SPS_O_DESC_DONE = 0x00000001,  
	SPS_O_INACTIVE  = 0x00000002,  
	SPS_O_WAKEUP    = 0x00000004,  
	SPS_O_OUT_OF_DESC = 0x00000008,
	SPS_O_ERROR     = 0x00000010,  
	SPS_O_EOT       = 0x00000020,  

	
	SPS_O_STREAMING = 0x00010000,  
	
	SPS_O_IRQ_MTI   = 0x00020000,
	/* NWD bit written with EOT for BAM2BAM producer pipe */
	SPS_O_WRITE_NWD   = 0x00040000,

	
	
	SPS_O_POLL      = 0x01000000,
	
	SPS_O_NO_Q      = 0x02000000,
	SPS_O_FLOWOFF   = 0x04000000,  
	
	SPS_O_WAKEUP_IS_ONESHOT = 0x08000000,
	SPS_O_ACK_TRANSFERS = 0x10000000,
	
	SPS_O_AUTO_ENABLE = 0x20000000,
	
	SPS_O_NO_EP_SYNC = 0x40000000,
};

enum sps_dma_priority {
	SPS_DMA_PRI_DEFAULT = 0,
	SPS_DMA_PRI_LOW,
	SPS_DMA_PRI_MED,
	SPS_DMA_PRI_HIGH,
};

enum sps_owner {
	SPS_OWNER_LOCAL = 0x1,	
	SPS_OWNER_REMOTE = 0x2,	
};

enum sps_event {
	SPS_EVENT_INVALID = 0,

	SPS_EVENT_EOT,		
	SPS_EVENT_DESC_DONE,	
	SPS_EVENT_OUT_OF_DESC,	
	SPS_EVENT_WAKEUP,	
	SPS_EVENT_FLOWOFF,	
	SPS_EVENT_INACTIVE,	
	SPS_EVENT_ERROR,	
	SPS_EVENT_MAX,
};

enum sps_trigger {
	
	SPS_TRIGGER_CALLBACK = 0,
	
	SPS_TRIGGER_WAIT,
};

enum sps_flow_off {
	SPS_FLOWOFF_FORCED = 0,	
	
	SPS_FLOWOFF_GRACEFUL,
};

enum sps_mem {
	SPS_MEM_LOCAL = 0,  
	SPS_MEM_UC,	    
};

enum sps_timer_op {
	SPS_TIMER_OP_CONFIG = 0,
	SPS_TIMER_OP_RESET,
	SPS_TIMER_OP_READ,
};

enum sps_timer_mode {
	SPS_TIMER_MODE_ONESHOT = 0,
};

enum sps_callback_case {
	SPS_CALLBACK_BAM_ERROR_IRQ = 1,     
	SPS_CALLBACK_BAM_HRESP_ERR_IRQ,	    
};

enum sps_command_type {
	SPS_WRITE_COMMAND = 0,
	SPS_READ_COMMAND,
};

struct sps_iovec {
	u32 addr;
	u32 size:16;
	u32 flags:16;
};

/**
 * This data type corresponds to the native Command Element
 * supported by SPS hardware
 *
 * @addr - register address.
 * @command - command type.
 * @data - for write command: content to be written into peripheral register.
 *         for read command: dest addr to write peripheral register value to.
 * @mask - register mask.
 * @reserved - for future usage.
 *
 */
struct sps_command_element {
	u32 addr:24;
	u32 command:8;
	u32 data;
	u32 mask;
	u32 reserved;
};

struct sps_bam_pipe_sec_config_props {
	u32 pipe_mask;
	u32 vmid;
};

struct sps_bam_sec_config_props {
	
	struct sps_bam_pipe_sec_config_props ees[SPS_BAM_NUM_EES];
};

struct sps_bam_props {

	

	u32 options;
	u32 phys_addr;
	void *virt_addr;
	u32 virt_size;
	u32 irq;
	u32 num_pipes;
	u32 summing_threshold;

	

	u32 periph_class;
	u32 periph_dev_id;
	u32 periph_phys_addr;
	void *periph_virt_addr;
	u32 periph_virt_size;

	

	u32 event_threshold;
	u32 desc_size;
	u32 data_size;
	u32 desc_mem_id;
	u32 data_mem_id;

	
	void (*callback)(enum sps_callback_case, void *);
	void *user;

	

	u32 manage;
	u32 restricted_pipes;
	u32 ee;

	

	u32 irq_gen_addr;

	

	u32 sec_config;
	struct sps_bam_sec_config_props *p_sec_config_props;
};

struct sps_mem_buffer {
	void *base;
	u32 phys_base;
	u32 size;
	u32 min_size;
};

struct sps_connect {
	u32 source;
	u32 src_pipe_index;
	u32 destination;
	u32 dest_pipe_index;

	enum sps_mode mode;

	u32 config;

	enum sps_option options;

	struct sps_mem_buffer desc;
	struct sps_mem_buffer data;

	u32 event_thresh;

	u32 lock_group;

	

	u32 irq_gen_addr;
	u32 irq_gen_data;

	u32 sps_reserved;

};

struct sps_satellite {
	u32 dev;
	u32 pipe_index;

	u32 config;
	enum sps_option options;

};

struct sps_alloc_dma_chan {
	u32 dev;

	

	u32 threshold;
	enum sps_dma_priority priority;

	u32 src_owner;
	u32 dest_owner;

};

struct sps_dma_chan {
	u32 dev;
	u32 dest_pipe_index;
	u32 src_pipe_index;
};

/**
 * This struct is an argument passed payload when triggering a callback event
 * object registered for an SPS connection end point.
 *
 * @user - Pointer registered with sps_register_event().
 *
 * @event_id - Which event.
 *
 * @iovec - The associated I/O vector. If the end point is a system-mode
 * producer, the size will reflect the actual number of bytes written to the
 * buffer by the pipe. NOTE: If this I/O vector was part of a set submitted to
 * sps_transfer(), then the vector array itself will be	updated with all of
 * the actual counts.
 *
 * @user - Pointer registered with the transfer.
 *
 */
struct sps_event_notify {
	void *user;

	enum sps_event event_id;

	

	union {
		
		struct {
			u32 mask;
		} irq;

		

		struct {
			struct sps_iovec iovec;
			void *user;
		} transfer;

		

		struct {
			u32 status;
		} err;

	} data;
};

struct sps_register_event {
	enum sps_option options;
	enum sps_trigger mode;
	struct completion *xfer_done;
	void (*callback)(struct sps_event_notify *notify);
	void *user;
};

struct sps_transfer {
	u32 iovec_phys;
	struct sps_iovec *iovec;
	u32 iovec_count;
	void *user;
};

struct sps_timer_ctrl {
	enum sps_timer_op op;

	enum sps_timer_mode mode;
	u32 timeout_msec;
};

struct sps_timer_result {
	u32 current_timer;
};


struct sps_pipe;	

#ifdef CONFIG_SPS
/**
 * Register a BAM device
 *
 * This function registers a BAM device with the SPS driver. For each
 *peripheral that includes a BAM, the peripheral driver must register
 * the BAM with the SPS driver.
 *
 * A requirement is that the peripheral driver must remain attached
 * to the SPS driver until the BAM is deregistered. Otherwise, the
 * system may attempt to unload the SPS driver. BAM registrations would
 * be lost.
 *
 * @bam_props - Pointer to struct for BAM device properties.
 *
 * @dev_handle - Device handle will be written to this location (output).
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_register_bam_device(const struct sps_bam_props *bam_props,
			    u32 *dev_handle);

/**
 * Deregister a BAM device
 *
 * This function deregisters a BAM device from the SPS driver. The peripheral
 * driver should deregister a BAM when the peripheral driver is shut down or
 * when BAM use should be disabled.
 *
 * A BAM cannot be deregistered if any of its pipes is in an active connection.
 *
 * When all BAMs have been deregistered, the system is free to unload the
 * SPS driver.
 *
 * @dev_handle - BAM device handle.
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_deregister_bam_device(u32 dev_handle);

/**
 * Allocate client state context
 *
 * This function allocate and initializes a client state context struct.
 *
 * @return pointer to client state context
 *
 */
struct sps_pipe *sps_alloc_endpoint(void);

/**
 * Free client state context
 *
 * This function de-initializes and free a client state context struct.
 *
 * @ctx - client context for SPS connection end point
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_free_endpoint(struct sps_pipe *h);

/**
 * Get the configuration parameters for an SPS connection end point
 *
 * This function retrieves the configuration parameters for an SPS connection
 * end point.
 * This function may be called before the end point is connected (before
 * sps_connect is called). This allows the client to specify parameters before
 * the connection is established.
 *
 * The client must call this function to fill it's struct sps_connect
 * struct before modifying values and passing the struct to sps_set_config().
 *
 * @h - client context for SPS connection end point
 *
 * @config - Pointer to buffer for the end point's configuration parameters.
 * Must not be NULL.
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_get_config(struct sps_pipe *h, struct sps_connect *config);

/**
 * Allocate memory from the SPS Pipe-Memory.
 *
 * @h - client context for SPS connection end point
 *
 * @mem - memory type - N/A.
 *
 * @mem_buffer - Pointer to struct for allocated memory properties.
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_alloc_mem(struct sps_pipe *h, enum sps_mem mem,
		  struct sps_mem_buffer *mem_buffer);

/**
 * Free memory from the SPS Pipe-Memory.
 *
 * @h - client context for SPS connection end point
 *
 * @mem_buffer - Pointer to struct for allocated memory properties.
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_free_mem(struct sps_pipe *h, struct sps_mem_buffer *mem_buffer);

/**
 * Connect an SPS connection end point
 *
 * This function creates a connection between two SPS peripherals or between
 * an SPS peripheral and the local host processor (via system memory, end
 *point SPS_DEV_HANDLE_MEM). Establishing the connection includes
 * initialization of the SPS hardware and allocation of any other connection
 * resources (buffer memory, etc.).
 *
 * This function requires the client to specify both the source and
 * destination end points of the SPS connection. However, the handle
 * returned applies only to the end point of the connection that the client
 * controls. The end point under control must be specified by the
 * enum sps_mode mode argument, either SPS_MODE_SRC, SPS_MODE_DEST, or
 * SPS_MODE_CTL. Note that SPS_MODE_CTL is only supported for I/O
 * accelerator connections, and only a limited set of control operations are
 * allowed (TBD).
 *
 * For a connection involving system memory
 * (SPS_DEV_HANDLE_MEM), the peripheral end point must be
 * specified. For example, SPS_MODE_SRC must be specified for a
 * BAM-to-system connection, since the BAM pipe is the data
 * producer.
 *
 * For a specific peripheral-to-peripheral connection, there may be more than
 * one required configuration. For example, there might be high-performance
 * and low-power configurations for a connection between the two peripherals.
 * The config argument allows the client to specify different configurations,
 * which may require different system resource allocations and hardware
 * initialization.
 *
 * A client is allowed to create one and only one connection for its
 * struct sps_pipe. The handle is used to identify the connection end point
 * in subsequent SPS driver calls. A specific connection source or
 * destination end point can be associated with one and only one
 * struct sps_pipe.
 *
 * The client must establish an open device handle to the SPS. To do so, the
 * client must attach to the SPS driver and open the SPS device by calling
 * the following functions.
 *
 * @h - client context for SPS connection end point
 *
 * @connect - Pointer to connection parameters
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_connect(struct sps_pipe *h, struct sps_connect *connect);

/**
 * Disconnect an SPS connection end point
 *
 * This function disconnects an SPS connection end point.
 * The SPS hardware associated with that end point will be disabled.
 * For a connection involving system memory (SPS_DEV_HANDLE_MEM), all
 * connection resources are deallocated. For a peripheral-to-peripheral
 * connection, the resources associated with the connection will not be
 * deallocated until both end points are closed.
 *
 * The client must call sps_connect() for the handle before calling
 * this function.
 *
 * @h - client context for SPS connection end point
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_disconnect(struct sps_pipe *h);

/**
 * Register an event object for an SPS connection end point
 *
 * This function registers a callback event object for an SPS connection end
 *point. The registered event object will be triggered for the set of
 * events specified in reg->options that are enabled for the end point.
 *
 * There can only be one registered event object for each event. If an event
 * object is already registered for an event, it will be replaced. If
 *reg->event handle is NULL, then any registered event object for the
 * event will be deregistered. Option bits in reg->options not associated
 * with events are ignored.
 *
 * The client must call sps_connect() for the handle before calling
 * this function.
 *
 * @h - client context for SPS connection end point
 *
 * @reg - Pointer to event registration parameters
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_register_event(struct sps_pipe *h, struct sps_register_event *reg);

/**
 * Perform a single DMA transfer on an SPS connection end point
 *
 * This function submits a DMA transfer request consisting of a single buffer
 * for an SPS connection end point associated with a peripheral-to/from-memory
 * connection. The request will be submitted immediately to hardware if the
 * hardware is idle (data flow off, no other pending transfers). Otherwise, it
 * will be queued for later handling in the SPS driver work loop.
 *
 * The data buffer must be DMA ready. The client is responsible for insuring
 *physically contiguous memory, cache maintenance, and memory barrier. For
 * more information, see Appendix A.
 *
 * The client must not modify the data buffer until the completion indication is
 * received.
 *
 * This function cannot be used if transfer queuing is disabled (see option
 * SPS_O_NO_Q). The client must set the SPS_O_EOT option to receive a callback
 * event trigger when the transfer is complete. The SPS driver will insure the
 * appropriate flags in the I/O vectors are set to generate the completion
 * indication.
 *
 * The return value from this function may indicate that an error occurred.
 * Possible causes include invalid arguments.
 *
 * @h - client context for SPS connection end point
 *
 * @addr - Physical address of buffer to transfer.
 *
 * WARNING: The memory provided	should be physically contiguous and
 * non-cached.
 *
 * The user can use one of the following:
 * 1. sps_alloc_mem() - allocated from pipe-memory.
 * 2. dma_alloc_coherent() - allocate DMA memory.
 * 3. dma_map_single() for memory allocated by kmalloc().
 *
 * @size - Size in bytes of buffer to transfer
 *
 * @user - User pointer that will be returned to user as part of
 *  event payload
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_transfer_one(struct sps_pipe *h, u32 addr, u32 size,
		     void *user, u32 flags);

/**
 * Read event queue for an SPS connection end point
 *
 * This function reads event queue for an SPS connection end point.
 *
 * @h - client context for SPS connection end point
 *
 * @event - pointer to client's event data buffer
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_get_event(struct sps_pipe *h, struct sps_event_notify *event);

/**
 * Get processed I/O vector (completed transfers)
 *
 * This function fetches the next processed I/O vector.
 *
 * @h - client context for SPS connection end point
 *
 * @iovec - Pointer to I/O vector struct (output).
 * This struct will be zeroed if there are no more processed I/O vectors.
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_get_iovec(struct sps_pipe *h, struct sps_iovec *iovec);

/**
 * Enable an SPS connection end point
 *
 * This function enables an SPS connection end point.
 *
 * @h - client context for SPS connection end point
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_flow_on(struct sps_pipe *h);

/**
 * Disable an SPS connection end point
 *
 * This function disables an SPS connection end point.
 *
 * @h - client context for SPS connection end point
 *
 * @mode - Desired mode for disabling pipe data flow
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_flow_off(struct sps_pipe *h, enum sps_flow_off mode);

/**
 * Perform a Multiple DMA transfer on an SPS connection end point
 *
 * This function submits a DMA transfer request for an SPS connection end point
 * associated with a peripheral-to/from-memory connection. The request will be
 * submitted immediately to hardware if the hardware is idle (data flow off, no
 * other pending transfers). Otherwise, it will be queued for later handling in
 * the SPS driver work loop.
 *
 * The data buffers referenced by the I/O vectors must be DMA ready.
 * The client is responsible for insuring physically contiguous memory,
 * any cache maintenance, and memory barrier. For more information,
 * see Appendix A.
 *
 * The I/O vectors must specify physical addresses for the referenced buffers.
 *
 * The client must not modify the data buffers referenced by I/O vectors until
 * the completion indication is received.
 *
 * If transfer queuing is disabled (see option SPS_O_NO_Q), the client is
 * responsible for setting the appropriate flags in the I/O vectors to generate
 * the completion indication. Also, the client is responsible for enabling the
 * appropriate connection callback event options for completion indication (see
 * sps_connect(), sps_set_config()).
 *
 * If transfer queuing is enabled, the client must set the SPS_O_EOT option to
 * receive a callback event trigger when the transfer is complete. The SPS
 * driver will insure the appropriate flags in the I/O vectors are set to
 * generate the completion indication. The client must not set any flags in the
 * I/O vectors, as this may cause the SPS driver to become out of sync with the
 * hardware.
 *
 * The return value from this function may indicate that an error occurred.
 * Possible causes include invalid arguments. If transfer queuing is disabled,
 * an error will occur if the pipe is already processing a transfer.
 *
 * @h - client context for SPS connection end point
 *
 * @transfer - Pointer to transfer parameter struct
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_transfer(struct sps_pipe *h, struct sps_transfer *transfer);

/**
 * Determine whether an SPS connection end point FIFO is empty
 *
 * This function returns the empty state of an SPS connection end point.
 *
 * @h - client context for SPS connection end point
 *
 * @empty - pointer to client's empty status word (boolean)
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_is_pipe_empty(struct sps_pipe *h, u32 *empty);

/**
 * Reset an SPS BAM device
 *
 * This function resets an SPS BAM device.
 *
 * @dev - device handle for the BAM
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_device_reset(u32 dev);

/**
 * Set the configuration parameters for an SPS connection end point
 *
 * This function sets the configuration parameters for an SPS connection
 * end point. This function may be called before the end point is connected
 * (before sps_connect is called). This allows the client to specify
 *parameters before the connection is established. The client is allowed
 * to pre-allocate resources and override driver defaults.
 *
 * The client must call sps_get_config() to fill it's struct sps_connect
 * struct before modifying values and passing the struct to this function.
 * Only those parameters that differ from the current configuration will
 * be processed.
 *
 * @h - client context for SPS connection end point
 *
 * @config - Pointer to the end point's new configuration parameters.
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_set_config(struct sps_pipe *h, struct sps_connect *config);

/**
 * Set ownership of an SPS connection end point
 *
 * This function sets the ownership of an SPS connection end point to
 * either local (default) or non-local. This function is used to
 * retrieve the struct sps_connect data that must be used by a
 * satellite processor when calling sps_connect().
 *
 * Non-local ownership is only possible/meaningful on the processor
 * that controls resource allocations (apps processor). Setting ownership
 * to non-local on a satellite processor will fail.
 *
 * Setting ownership from non-local to local will succeed only if the
 * owning satellite processor has properly brought the end point to
 * an idle condition.
 *
 * This function will succeed if the connection end point is already in
 * the specified ownership state.
 *
 * @h - client context for SPS connection end point
 *
 * @owner - New ownership of the connection end point
 *
 * @connect - Pointer to buffer for satellite processor connect data.
 *  Can be NULL to avoid retrieving the connect data. Will be ignored
 *  if the end point ownership is set to local.
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_set_owner(struct sps_pipe *h, enum sps_owner owner,
		  struct sps_satellite *connect);

/**
 * Allocate a BAM DMA channel
 *
 * This function allocates a BAM DMA channel. A "BAM DMA" is a special
 * DMA peripheral with a BAM front end. The DMA peripheral acts as a conduit
 * for data to flow into a consumer pipe and then out of a producer pipe.
 * It's primarily purpose is to serve as a path for interprocessor communication
 * that allows each processor to control and protect it's own memory space.
 *
 * @alloc - Pointer to struct for BAM DMA channel allocation properties.
 *
 * @chan - Allocated channel information will be written to this
 *  location (output).
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_alloc_dma_chan(const struct sps_alloc_dma_chan *alloc,
		       struct sps_dma_chan *chan);

/**
 * Free a BAM DMA channel
 *
 * This function frees a BAM DMA channel.
 *
 * @chan - Pointer to information for channel to free
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_free_dma_chan(struct sps_dma_chan *chan);

/**
 * Get the BAM handle for BAM-DMA.
 *
 * The BAM handle should be use as source/destination in the sps_connect().
 *
 * @return handle on success, zero on error
 *
 */
u32 sps_dma_get_bam_handle(void);

/**
 * Free the BAM handle for BAM-DMA.
 *
 */
void sps_dma_free_bam_handle(u32 h);


/**
 * Get number of free transfer entries for an SPS connection end point
 *
 * This function returns the number of free transfer entries for an
 * SPS connection end point.
 *
 * @h - client context for SPS connection end point
 *
 * @count - pointer to count status
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_get_free_count(struct sps_pipe *h, u32 *count);

/**
 * Perform timer control
 *
 * This function performs timer control operations.
 *
 * @h - client context for SPS connection end point
 *
 * @timer_ctrl - Pointer to timer control specification
 *
 * @timer_result - Pointer to buffer for timer operation result.
 *  This argument can be NULL if no result is expected for the operation.
 *  If non-NULL, the current timer value will always provided.
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_timer_ctrl(struct sps_pipe *h,
		   struct sps_timer_ctrl *timer_ctrl,
		   struct sps_timer_result *timer_result);

/**
 * Find the handle of a BAM device based on the physical address
 *
 * This function finds a BAM device in the BAM registration list that
 * matches the specified physical address, and returns its handle.
 *
 * @phys_addr - physical address of the BAM
 *
 * @h - device handle of the BAM
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_phy2h(u32 phys_addr, u32 *handle);

/**
 * Setup desc/data FIFO for bam-to-bam connection
 *
 * @mem_buffer - Pointer to struct for allocated memory properties.
 *
 * @addr - address of FIFO
 *
 * @size - FIFO size
 *
 * @use_offset - use address offset instead of absolute address
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_setup_bam2bam_fifo(struct sps_mem_buffer *mem_buffer,
		  u32 addr, u32 size, int use_offset);

/**
 * Get the number of unused descriptors in the descriptor FIFO
 * of a pipe
 *
 * @h - client context for SPS connection end point
 *
 * @desc_num - number of unused descriptors
 *
 * @return 0 on success, negative value on error
 *
 */
int sps_get_unused_desc_num(struct sps_pipe *h, u32 *desc_num);

int sps_get_bam_debug_info(u32 dev, u32 option);

#else
static inline int sps_register_bam_device(const struct sps_bam_props
			*bam_props, u32 *dev_handle)
{
	return -EPERM;
}

static inline int sps_deregister_bam_device(u32 dev_handle)
{
	return -EPERM;
}

static inline struct sps_pipe *sps_alloc_endpoint(void)
{
	return NULL;
}

static inline int sps_free_endpoint(struct sps_pipe *h)
{
	return -EPERM;
}

static inline int sps_get_config(struct sps_pipe *h, struct sps_connect *config)
{
	return -EPERM;
}

static inline int sps_alloc_mem(struct sps_pipe *h, enum sps_mem mem,
		  struct sps_mem_buffer *mem_buffer)
{
	return -EPERM;
}

static inline int sps_free_mem(struct sps_pipe *h,
				struct sps_mem_buffer *mem_buffer)
{
	return -EPERM;
}

static inline int sps_connect(struct sps_pipe *h, struct sps_connect *connect)
{
	return -EPERM;
}

static inline int sps_disconnect(struct sps_pipe *h)
{
	return -EPERM;
}

static inline int sps_register_event(struct sps_pipe *h,
					struct sps_register_event *reg)
{
	return -EPERM;
}

static inline int sps_transfer_one(struct sps_pipe *h, u32 addr, u32 size,
		     void *user, u32 flags)
{
	return -EPERM;
}

static inline int sps_get_event(struct sps_pipe *h,
				struct sps_event_notify *event)
{
	return -EPERM;
}

static inline int sps_get_iovec(struct sps_pipe *h, struct sps_iovec *iovec)
{
	return -EPERM;
}

static inline int sps_flow_on(struct sps_pipe *h)
{
	return -EPERM;
}

static inline int sps_flow_off(struct sps_pipe *h, enum sps_flow_off mode)
{
	return -EPERM;
}

static inline int sps_transfer(struct sps_pipe *h,
				struct sps_transfer *transfer)
{
	return -EPERM;
}

static inline int sps_is_pipe_empty(struct sps_pipe *h, u32 *empty)
{
	return -EPERM;
}

static inline int sps_device_reset(u32 dev)
{
	return -EPERM;
}

static inline int sps_set_config(struct sps_pipe *h, struct sps_connect *config)
{
	return -EPERM;
}

static inline int sps_set_owner(struct sps_pipe *h, enum sps_owner owner,
		  struct sps_satellite *connect)
{
	return -EPERM;
}

static inline int sps_get_free_count(struct sps_pipe *h, u32 *count)
{
	return -EPERM;
}

static inline int sps_alloc_dma_chan(const struct sps_alloc_dma_chan *alloc,
		       struct sps_dma_chan *chan)
{
	return -EPERM;
}

static inline int sps_free_dma_chan(struct sps_dma_chan *chan)
{
	return -EPERM;
}

static inline u32 sps_dma_get_bam_handle(void)
{
	return 0;
}

static inline void sps_dma_free_bam_handle(u32 h)
{
}

static inline int sps_timer_ctrl(struct sps_pipe *h,
		   struct sps_timer_ctrl *timer_ctrl,
		   struct sps_timer_result *timer_result)
{
	return -EPERM;
}

static inline int sps_phy2h(u32 phys_addr, u32 *handle)
{
	return -EPERM;
}

static inline int sps_setup_bam2bam_fifo(struct sps_mem_buffer *mem_buffer,
		  u32 addr, u32 size, int use_offset)
{
	return -EPERM;
}

static inline int sps_get_unused_desc_num(struct sps_pipe *h, u32 *desc_num)
{
	return -EPERM;
}
#endif

#endif /* _SPS_H_ */
