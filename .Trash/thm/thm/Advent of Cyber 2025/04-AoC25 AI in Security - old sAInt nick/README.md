---
title: Advent of Cyber 2025 - AI in Security - old SAInt nick (TryHackMe)
tags:
  - cybersecurity
  - thm
  - advent_of_cyber
  - artificial_intelligence
slug: aoc25-AI in Security
date: 2025-12-05
summary:
---

# Advent of Cyber 2025 — Day 04  

---
## TL;DR

Day 04 introduces _Van SolveIT_, a prototype AI security assistant used for red-team exploitation, blue-team log analysis, and secure-coding review. The core lesson is simple: **AI is a force multiplier, not an authority**—use it to accelerate work, but never trust its output blindly.

---

## Scenario / Objective
The challenge takes place in Wareville, where The Best Festival Company (TBFC) is piloting a cybersecurity AI assistant called **Van SolveIT**. You interact with the assistant through a web interface hosted on the target machine and access it via the AttackBox.

Van SolveIT showcases three security workflows:
- **Red Team** – Generate an exploit script against a vulnerable web application and retrieve a flag.
- **Blue Team** – Analyse web server logs to identify malicious activity.
- **Software Security** – Review source code to identify vulnerabilities.    

The objective is to complete all three showcases, follow the AI’s guidance where appropriate, and collect the flags. Practical constraints include slow AI responses and the need to restart conversations if the model drifts or produces unusable output.

---

## What Actually Matters Here
AI is increasingly embedded in cybersecurity because it can rapidly process data, summarise behaviour, and generate hypotheses that would otherwise take humans far longer to produce.

- **Defensive use**: AI can sift through massive volumes of logs and telemetry, surface anomalies, and assist incident responders by narrowing the search space.
- **Offensive use**: AI can automate reconnaissance, draft exploit scripts, and accelerate payload development.
- **Development use**: AI can act as a first-pass code auditor, identifying common vulnerability patterns and insecure practices.

That said, AI is **not reliable or authoritative**.

Language models can:
- Hallucinate vulnerabilities that don’t exist
- Miss critical context in logs
- Generate exploit code that is broken or insecure
- Encourage unsafe practices if prompted poorly    

From a security perspective, AI output must be treated as **untrusted input**. Human validation is mandatory. This exercise reinforces a key real-world principle: **AI augments expertise—it does not replace it**.

---

## Walkthrough (High Signal Only)

### Step 1 — Accessing Van SolveIT
Deploy the AttackBox and the target VM. Open a browser and navigate to the target machine’s IP address to access the Van SolveIT interface.

![[Pasted image 20260105165659.png]]

---

### Step 2 — Red-Team Showcase
Select the **Red** showcase and prompt Van SolveIT to generate an exploit for the vulnerable web application. The AI provides a Python script containing a SQL injection payload.

Actions required:
- Replace the placeholder IP address in the script with the target VM’s IP
- Execute the script from the AttackBox    
- Capture the returned flag

📸 Screenshot suggestions:
- AI conversation showing exploit generation (`ai-exploit-generation.png`)
- Terminal output confirming successful execution (optional)    

---

### Step 3 — Blue-Team Showcase
Switch to the **Blue** showcase and provide the requested web server logs. Van SolveIT assists in identifying suspicious activity by suggesting command-line filters and patterns.

Look for:
- Repeated authentication failures
- Abnormal request paths
- Input indicative of injection or traversal attempts

⚠️ **Important:** AI-generated queries can miss context or surface false positives. Always sanity-check results and refine filters manually.

📸 Screenshot suggestion:

- AI conversation highlighting malicious log entries (`ai-log-analysis.png`)
    

---

### Step 4 — Software Showcase
Choose the **Software** showcase and submit the provided source code for review. Van SolveIT analyses the code and highlights insecure patterns such as unsanitised input or vulnerable database queries.

The value here is not the exact findings, but **how quickly AI can flag common classes of issues** for human review.

📸 Screenshot suggestion:

- AI explanation of identified vulnerabilities (`ai-code-review.png`)
    

---

### Step 5 — Collecting Flags
After completing the showcases, the following flags are obtained:

- AI showcase completion flag: `THM{AI_MANIA}`
    
- Red-team exploit flag: `THM{SQLI_EXPLOIT}`
    

Submit the flags to complete the challenge.

---

## Key Takeaways
**Technical**
- AI assistants can dramatically reduce time spent on repetitive security tasks such as exploit drafting, log triage, and initial code review.
**Security**
- AI introduces new risks: hallucinations, insecure suggestions, and data-leak potential. Treat AI output as untrusted until verified.
**Operational**
- AI should be integrated as a helper, not a decision-maker. Human oversight remains non-negotiable.    

---

## Real-World Relevance
AI-driven tooling is already embedded in modern security operations—from SIEM platforms using anomaly detection to pentesters using language models for rapid exploit prototyping. Attackers are also leveraging AI for phishing, reconnaissance, and automation.

This challenge demonstrates both sides of that reality. Used responsibly, AI accelerates detection, response, and development. Used carelessly, it becomes a liability. Effective organisations pair AI with strong access controls, output validation, and continuous monitoring—ensuring speed without sacrificing security.