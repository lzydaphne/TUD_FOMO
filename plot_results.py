#!/usr/bin/env python3
"""
Plot benchmark results for FOMO Project 1 Report
Reads benchmark_results.csv and creates bandwidth and latency plots
"""

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Read the CSV file
print("Reading benchmark_results.csv...")
df = pd.read_csv('benchmark_results.csv')

# Separate Sequential and Random data
df_seq = df[df['pattern'] == 'Sequential']
df_rand = df[df['pattern'] == 'Random']

# Create figure with 2 subplots
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 6))

# Color scheme
colors = {
    'DirectAccess': '#1f77b4',
    'BinarySearch': '#ff7f0e',
    'Hash(bin=1)': '#2ca02c',
    'Hash(bin=16)': '#d62728'
}

# ==================== BANDWIDTH PLOT ====================
ax1.set_title('Bandwidth - Sequential vs Random Access', fontsize=14, fontweight='bold')
ax1.set_xlabel('Dataset Size (KB)', fontsize=12)
ax1.set_ylabel('Bandwidth (MB/s)', fontsize=12)
ax1.set_xscale('log')
ax1.set_yscale('log')
ax1.grid(True, alpha=0.3, which='both')

# Plot Sequential (solid lines)
ax1.plot(df_seq['size_kb'], df_seq['bw_directaccess'], 'o-',
         color=colors['DirectAccess'], linewidth=2, markersize=6,
         label='DirectAccess-Seq')
ax1.plot(df_seq['size_kb'], df_seq['bw_binarysearch'], 's-',
         color=colors['BinarySearch'], linewidth=2, markersize=6,
         label='BinarySearch-Seq')
ax1.plot(df_seq['size_kb'], df_seq['bw_hash1'], '^-',
         color=colors['Hash(bin=1)'], linewidth=2, markersize=6,
         label='Hash(bin=1)-Seq')
ax1.plot(df_seq['size_kb'], df_seq['bw_hash16'], 'd-',
         color=colors['Hash(bin=16)'], linewidth=2, markersize=6,
         label='Hash(bin=16)-Seq')

# Plot Random (dashed lines)
ax1.plot(df_rand['size_kb'], df_rand['bw_directaccess'], 'o--',
         color=colors['DirectAccess'], linewidth=2, markersize=6, alpha=0.7,
         label='DirectAccess-Rand')
ax1.plot(df_rand['size_kb'], df_rand['bw_binarysearch'], 's--',
         color=colors['BinarySearch'], linewidth=2, markersize=6, alpha=0.7,
         label='BinarySearch-Rand')
ax1.plot(df_rand['size_kb'], df_rand['bw_hash1'], '^--',
         color=colors['Hash(bin=1)'], linewidth=2, markersize=6, alpha=0.7,
         label='Hash(bin=1)-Rand')
ax1.plot(df_rand['size_kb'], df_rand['bw_hash16'], 'd--',
         color=colors['Hash(bin=16)'], linewidth=2, markersize=6, alpha=0.7,
         label='Hash(bin=16)-Rand')

ax1.legend(loc='best', fontsize=9, ncol=2)

# Add cache level annotations
ax1.axvline(x=32, color='gray', linestyle=':', alpha=0.5)
ax1.text(32, ax1.get_ylim()[0]*1.5, 'L1', fontsize=8, alpha=0.7)
ax1.axvline(x=256, color='gray', linestyle=':', alpha=0.5)
ax1.text(256, ax1.get_ylim()[0]*1.5, 'L2', fontsize=8, alpha=0.7)
ax1.axvline(x=10000, color='gray', linestyle=':', alpha=0.5)
ax1.text(10000, ax1.get_ylim()[0]*1.5, 'L3', fontsize=8, alpha=0.7)

# ==================== LATENCY PLOT ====================
ax2.set_title('Latency - Sequential vs Random Access', fontsize=14, fontweight='bold')
ax2.set_xlabel('Dataset Size (KB)', fontsize=12)
ax2.set_ylabel('Latency (cycles/lookup)', fontsize=12)
ax2.set_xscale('log')
ax2.set_yscale('log')
ax2.grid(True, alpha=0.3, which='both')

# Plot Sequential (solid lines)
ax2.plot(df_seq['size_kb'], df_seq['lat_directaccess'], 'o-',
         color=colors['DirectAccess'], linewidth=2, markersize=6,
         label='DirectAccess-Seq')
ax2.plot(df_seq['size_kb'], df_seq['lat_binarysearch'], 's-',
         color=colors['BinarySearch'], linewidth=2, markersize=6,
         label='BinarySearch-Seq')
ax2.plot(df_seq['size_kb'], df_seq['lat_hash1'], '^-',
         color=colors['Hash(bin=1)'], linewidth=2, markersize=6,
         label='Hash(bin=1)-Seq')
ax2.plot(df_seq['size_kb'], df_seq['lat_hash16'], 'd-',
         color=colors['Hash(bin=16)'], linewidth=2, markersize=6,
         label='Hash(bin=16)-Seq')

# Plot Random (dashed lines)
ax2.plot(df_rand['size_kb'], df_rand['lat_directaccess'], 'o--',
         color=colors['DirectAccess'], linewidth=2, markersize=6, alpha=0.7,
         label='DirectAccess-Rand')
ax2.plot(df_rand['size_kb'], df_rand['lat_binarysearch'], 's--',
         color=colors['BinarySearch'], linewidth=2, markersize=6, alpha=0.7,
         label='BinarySearch-Rand')
ax2.plot(df_rand['size_kb'], df_rand['lat_hash1'], '^--',
         color=colors['Hash(bin=1)'], linewidth=2, markersize=6, alpha=0.7,
         label='Hash(bin=1)-Rand')
ax2.plot(df_rand['size_kb'], df_rand['lat_hash16'], 'd--',
         color=colors['Hash(bin=16)'], linewidth=2, markersize=6, alpha=0.7,
         label='Hash(bin=16)-Rand')

ax2.legend(loc='best', fontsize=9, ncol=2)

# Add cache level annotations
ax2.axvline(x=32, color='gray', linestyle=':', alpha=0.5)
ax2.text(32, ax2.get_ylim()[0]*1.5, 'L1', fontsize=8, alpha=0.7)
ax2.axvline(x=256, color='gray', linestyle=':', alpha=0.5)
ax2.text(256, ax2.get_ylim()[0]*1.5, 'L2', fontsize=8, alpha=0.7)
ax2.axvline(x=10000, color='gray', linestyle=':', alpha=0.5)
ax2.text(10000, ax2.get_ylim()[0]*1.5, 'L3', fontsize=8, alpha=0.7)

plt.tight_layout()

# Save the plot
print("Saving plots...")
plt.savefig('benchmark_plots.pdf', dpi=300, bbox_inches='tight')
plt.savefig('benchmark_plots.png', dpi=300, bbox_inches='tight')
print("✓ Saved: benchmark_plots.pdf")
print("✓ Saved: benchmark_plots.png")

# Show the plot
print("\nDisplaying plots...")
plt.show()

# Print summary statistics
print("\n" + "="*70)
print("SUMMARY STATISTICS")
print("="*70)

print("\nSequential Access - DirectAccessArray:")
print(f"  Max Bandwidth: {df_seq['bw_directaccess'].max():.0f} MB/s")
print(f"  Min Latency: {df_seq['lat_directaccess'].min():.1f} cycles")

print("\nRandom Access - DirectAccessArray:")
print(f"  Max Bandwidth: {df_rand['bw_directaccess'].max():.0f} MB/s")
print(f"  Max Latency: {df_rand['lat_directaccess'].max():.1f} cycles")

print("\nPerformance Degradation (Sequential vs Random):")
seq_bw_max = df_seq['bw_directaccess'].max()
rand_bw_min = df_rand['bw_directaccess'].min()
if rand_bw_min > 0:
    print(f"  Bandwidth ratio: {seq_bw_max / rand_bw_min:.1f}x")

seq_lat_min = df_seq['lat_directaccess'].min()
rand_lat_max = df_rand['lat_directaccess'].max()
if seq_lat_min > 0:
    print(f"  Latency ratio: {rand_lat_max / seq_lat_min:.1f}x")

print("\n" + "="*70)
print("Plots ready! Use benchmark_plots.pdf in your report.")
print("="*70)
